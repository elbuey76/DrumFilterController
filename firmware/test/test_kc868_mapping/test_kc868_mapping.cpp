#include <unity.h>

#include "hal/Kc868Mapping.h"

namespace {
void setRawBit(Kc868DigitalInputsRaw& raw, uint8_t index, bool rawHigh) {
  const uint8_t bank = static_cast<uint8_t>(index / kKc868DigitalBitsPerBank);
  const uint8_t bit = static_cast<uint8_t>(index % kKc868DigitalBitsPerBank);
  const uint8_t mask = static_cast<uint8_t>(1U << bit);
  if (rawHigh) {
    raw.banks[bank] = static_cast<uint8_t>(raw.banks[bank] | mask);
  } else {
    raw.banks[bank] = static_cast<uint8_t>(raw.banks[bank] & static_cast<uint8_t>(~mask));
  }
}

Kc868DigitalInputsRaw healthyInputs() {
  Kc868DigitalInputsRaw raw;
  for (size_t index = 0; index < kKc868DigitalInputBankCount; ++index) {
    raw.bankOk[index] = true;
    raw.banks[index] = 0xFF;
  }
  return raw;
}

Kc868HardwareSafetyState armedSafety() {
  Kc868HardwareSafetyState safety;
  safety.armRequested = true;
  safety.profileValidated = true;
  safety.bootOffVerified = true;
  safety.inputBanksHealthy = true;
  safety.outputBanksHealthy = true;
  return safety;
}
}  // namespace

void setUp() {}

void tearDown() {}

void test_kc868_inputs_map_active_low_functional_order() {
  Kc868DigitalInputsRaw raw = healthyInputs();
  setRawBit(raw, 0, false);
  setRawBit(raw, 1, false);
  setRawBit(raw, 2, false);
  setRawBit(raw, 3, false);
  setRawBit(raw, 4, false);
  setRawBit(raw, 5, false);
  setRawBit(raw, 6, false);
  setRawBit(raw, 7, false);
  setRawBit(raw, 8, false);

  const InputsSnapshot inputs = kc868MapInputs(raw);

  TEST_ASSERT_TRUE(inputs.epLavage);
  TEST_ASSERT_TRUE(inputs.epCritique);
  TEST_ASSERT_TRUE(inputs.capotOuvert);
  TEST_ASSERT_TRUE(inputs.modeAuto);
  TEST_ASSERT_TRUE(inputs.modeMaintenance);
  TEST_ASSERT_TRUE(inputs.btnReset);
  TEST_ASSERT_TRUE(inputs.btnTestLavage);
  TEST_ASSERT_TRUE(inputs.btnManuTambour);
  TEST_ASSERT_TRUE(inputs.btnManuRincage);
  TEST_ASSERT_FALSE(inputs.tempBassinValid);
  TEST_ASSERT_FALSE(inputs.tempLocalValid);
}

void test_kc868_inputs_support_configurable_polarity() {
  Kc868DigitalMapping mapping = defaultKc868DigitalMapping();
  mapping.inputs[static_cast<uint8_t>(Kc868InputSignal::EP_LAVAGE)].activeLow = false;

  Kc868DigitalInputsRaw raw = healthyInputs();
  setRawBit(raw, 0, true);

  const InputsSnapshot inputs = kc868MapInputs(raw, mapping);

  TEST_ASSERT_TRUE(inputs.epLavage);
}

void test_kc868_input_bus_fault_returns_safe_inputs() {
  Kc868DigitalInputsRaw raw = healthyInputs();
  raw.bankOk[0] = false;

  const InputsSnapshot inputs = kc868MapInputs(raw);

  TEST_ASSERT_FALSE(inputs.modeAuto);
  TEST_ASSERT_TRUE(inputs.modeMaintenance);
  TEST_ASSERT_TRUE(inputs.capotOuvert);
}

void test_kc868_outputs_map_active_low_functional_order() {
  OutputsCommand outputs;
  outputs.cmdTambour = true;
  outputs.cmdUv = true;
  outputs.voyantAlarme = true;

  const Kc868DigitalOutputsRaw raw = kc868MapOutputs(outputs);

  TEST_ASSERT_BITS_LOW(0x01, raw.banks[0]);
  TEST_ASSERT_BITS_HIGH(0x02, raw.banks[0]);
  TEST_ASSERT_BITS_LOW(0x10, raw.banks[0]);
  TEST_ASSERT_BITS_LOW(0x01, raw.banks[1]);
}

void test_kc868_outputs_support_configurable_polarity() {
  Kc868DigitalMapping mapping = defaultKc868DigitalMapping();
  mapping.outputs[static_cast<uint8_t>(Kc868OutputSignal::CMD_TAMBOUR)].activeLow = false;
  OutputsCommand outputs;
  outputs.cmdTambour = true;

  const Kc868DigitalOutputsRaw raw = kc868MapOutputs(outputs, mapping);
  TEST_ASSERT_BITS_HIGH(0x01, raw.banks[0]);
}

void test_kc868_mapping_crosses_bank_boundary_at_input_and_output_nine() {
  Kc868DigitalInputsRaw raw = healthyInputs();
  setRawBit(raw, 7, false);
  setRawBit(raw, 8, false);

  const InputsSnapshot inputs = kc868MapInputs(raw);
  TEST_ASSERT_TRUE(inputs.btnManuTambour);
  TEST_ASSERT_TRUE(inputs.btnManuRincage);

  OutputsCommand outputs;
  outputs.voyantLavage = true;
  outputs.voyantAlarme = true;
  const Kc868DigitalOutputsRaw mapped = kc868MapOutputs(outputs);
  TEST_ASSERT_BITS_LOW(0x80, mapped.banks[0]);
  TEST_ASSERT_BITS_LOW(0x01, mapped.banks[1]);
}

void test_kc868_all_outputs_off_is_high_on_both_a16_banks() {
  const Kc868DigitalOutputsRaw raw = kc868AllOutputsOff();
  TEST_ASSERT_EQUAL_HEX8(0xFF, raw.banks[0]);
  TEST_ASSERT_EQUAL_HEX8(0xFF, raw.banks[1]);
}

void test_kc868_physical_output_diagnostic_pulse_addresses_each_of_the_16_y_terminals() {
  const Kc868DigitalOutputsRaw y1 = kc868PhysicalOutputDiagnosticPulse(1);
  TEST_ASSERT_EQUAL_HEX8(0xFE, y1.banks[0]);
  TEST_ASSERT_EQUAL_HEX8(0xFF, y1.banks[1]);

  const Kc868DigitalOutputsRaw y9 = kc868PhysicalOutputDiagnosticPulse(9);
  TEST_ASSERT_EQUAL_HEX8(0xFF, y9.banks[0]);
  TEST_ASSERT_EQUAL_HEX8(0xFE, y9.banks[1]);

  const Kc868DigitalOutputsRaw y16 = kc868PhysicalOutputDiagnosticPulse(16);
  TEST_ASSERT_EQUAL_HEX8(0xFF, y16.banks[0]);
  TEST_ASSERT_EQUAL_HEX8(0x7F, y16.banks[1]);
}

void test_kc868_outputs_are_forced_off_when_not_armed() {
  OutputsCommand requested;
  requested.cmdTambour = true;
  requested.cmdRincage = true;
  requested.voyantAlarme = true;

  Kc868HardwareSafetyState safety = armedSafety();
  safety.armRequested = false;
  const OutputsCommand effective = kc868EffectiveOutputs(requested, safety);

  TEST_ASSERT_FALSE(effective.cmdTambour);
  TEST_ASSERT_FALSE(effective.cmdRincage);
  TEST_ASSERT_FALSE(effective.voyantAlarme);
}

void test_kc868_outputs_are_forced_off_when_any_safety_gate_is_missing() {
  OutputsCommand requested;
  requested.cmdTambour = true;
  requested.cmdRincage = true;
  requested.voyantAlarme = true;

  Kc868HardwareSafetyState safety = armedSafety();
  bool* gates[] = {&safety.profileValidated, &safety.bootOffVerified, &safety.inputBanksHealthy, &safety.outputBanksHealthy};
  for (bool* gate : gates) {
    *gate = false;
    const OutputsCommand effective = kc868EffectiveOutputs(requested, safety);
    TEST_ASSERT_FALSE(effective.cmdTambour);
    TEST_ASSERT_FALSE(effective.cmdRincage);
    TEST_ASSERT_FALSE(effective.voyantAlarme);
    *gate = true;
  }
}

void test_kc868_outputs_pass_only_when_all_safety_gates_are_true() {
  OutputsCommand requested;
  requested.cmdTambour = true;
  requested.voyantAlarme = true;
  const OutputsCommand effective = kc868EffectiveOutputs(requested, armedSafety());
  TEST_ASSERT_TRUE(effective.cmdTambour);
  TEST_ASSERT_TRUE(effective.voyantAlarme);
}

void test_kc868_diagnostic_pulses_need_an_explicit_test_build_and_validated_boot_proof() {
  Kc868HardwareSafetyState safety = armedSafety();
  safety.armRequested = false;
  safety.profileValidated = false;
  TEST_ASSERT_FALSE(kc868DiagnosticPulsesPermitted(safety, true, false));
  TEST_ASSERT_TRUE(kc868DiagnosticPulsesPermitted(safety, true, true));

  safety.bootOffVerified = false;
  TEST_ASSERT_FALSE(kc868DiagnosticPulsesPermitted(safety, true, true));
  safety.bootOffVerified = true;
  TEST_ASSERT_FALSE(kc868DiagnosticPulsesPermitted(safety, false, true));
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  UNITY_BEGIN();
  RUN_TEST(test_kc868_inputs_map_active_low_functional_order);
  RUN_TEST(test_kc868_inputs_support_configurable_polarity);
  RUN_TEST(test_kc868_input_bus_fault_returns_safe_inputs);
  RUN_TEST(test_kc868_outputs_map_active_low_functional_order);
  RUN_TEST(test_kc868_outputs_support_configurable_polarity);
  RUN_TEST(test_kc868_mapping_crosses_bank_boundary_at_input_and_output_nine);
  RUN_TEST(test_kc868_all_outputs_off_is_high_on_both_a16_banks);
  RUN_TEST(test_kc868_physical_output_diagnostic_pulse_addresses_each_of_the_16_y_terminals);
  RUN_TEST(test_kc868_outputs_are_forced_off_when_not_armed);
  RUN_TEST(test_kc868_outputs_are_forced_off_when_any_safety_gate_is_missing);
  RUN_TEST(test_kc868_outputs_pass_only_when_all_safety_gates_are_true);
  RUN_TEST(test_kc868_diagnostic_pulses_need_an_explicit_test_build_and_validated_boot_proof);
  return UNITY_END();
}
