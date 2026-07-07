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
  for (size_t index = 0; index < kKc868DigitalBankCount; ++index) {
    raw.bankOk[index] = true;
    raw.banks[index] = 0xFF;
  }
  return raw;
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
  TEST_ASSERT_TRUE(inputs.tempBassinValid);
  TEST_ASSERT_TRUE(inputs.tempLocalValid);
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

void test_kc868_outputs_are_forced_off_when_not_armed() {
  OutputsCommand requested;
  requested.cmdTambour = true;
  requested.cmdRincage = true;
  requested.voyantAlarme = true;

  const OutputsCommand effective = kc868EffectiveOutputs(requested, false, true);

  TEST_ASSERT_FALSE(effective.cmdTambour);
  TEST_ASSERT_FALSE(effective.cmdRincage);
  TEST_ASSERT_FALSE(effective.voyantAlarme);
}

void test_kc868_outputs_are_forced_off_when_hardware_io_faulted() {
  OutputsCommand requested;
  requested.cmdTambour = true;
  requested.cmdRincage = true;
  requested.voyantAlarme = true;

  const OutputsCommand effective = kc868EffectiveOutputs(requested, true, false);

  TEST_ASSERT_FALSE(effective.cmdTambour);
  TEST_ASSERT_FALSE(effective.cmdRincage);
  TEST_ASSERT_FALSE(effective.voyantAlarme);
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  UNITY_BEGIN();
  RUN_TEST(test_kc868_inputs_map_active_low_functional_order);
  RUN_TEST(test_kc868_inputs_support_configurable_polarity);
  RUN_TEST(test_kc868_input_bus_fault_returns_safe_inputs);
  RUN_TEST(test_kc868_outputs_map_active_low_functional_order);
  RUN_TEST(test_kc868_outputs_are_forced_off_when_not_armed);
  RUN_TEST(test_kc868_outputs_are_forced_off_when_hardware_io_faulted);
  return UNITY_END();
}
