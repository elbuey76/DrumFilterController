#include <unity.h>

#include "sim/SimulatorCommands.h"

void setUp() {}

void tearDown() {}

void test_lavage_and_critical_commands_update_level_inputs() {
  InputsSnapshot inputs;

  SimulatorCommandResult result = applySimulatorCommand("lavage on", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_EP_LAVAGE, result.action);
  TEST_ASSERT_TRUE(result.boolValue);
  TEST_ASSERT_TRUE(inputs.epLavage);

  result = applySimulatorCommand("lavage off", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_EP_LAVAGE, result.action);
  TEST_ASSERT_FALSE(result.boolValue);
  TEST_ASSERT_FALSE(inputs.epLavage);

  result = applySimulatorCommand("critique on", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_EP_CRITIQUE, result.action);
  TEST_ASSERT_TRUE(inputs.epCritique);

  result = applySimulatorCommand("critique off", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_EP_CRITIQUE, result.action);
  TEST_ASSERT_FALSE(inputs.epCritique);
}

void test_capot_and_mode_commands_update_inputs() {
  InputsSnapshot inputs;

  SimulatorCommandResult result = applySimulatorCommand("capot open", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_CAPOT_OUVERT, result.action);
  TEST_ASSERT_TRUE(inputs.capotOuvert);

  result = applySimulatorCommand("capot close", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_CAPOT_OUVERT, result.action);
  TEST_ASSERT_FALSE(inputs.capotOuvert);

  result = applySimulatorCommand("maintenance", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::MODE_MAINTENANCE, result.action);
  TEST_ASSERT_FALSE(inputs.modeAuto);
  TEST_ASSERT_TRUE(inputs.modeMaintenance);

  result = applySimulatorCommand("manual", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::MODE_MANUAL, result.action);
  TEST_ASSERT_FALSE(inputs.modeAuto);
  TEST_ASSERT_FALSE(inputs.modeMaintenance);

  result = applySimulatorCommand("auto", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::MODE_AUTO, result.action);
  TEST_ASSERT_TRUE(inputs.modeAuto);
  TEST_ASSERT_FALSE(inputs.modeMaintenance);
}

void test_pulse_commands_set_one_shot_buttons() {
  InputsSnapshot inputs;

  SimulatorCommandResult result = applySimulatorCommand("reset", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::PULSE_RESET, result.action);
  TEST_ASSERT_TRUE(inputs.btnReset);

  result = applySimulatorCommand("test", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::PULSE_TEST_LAVAGE, result.action);
  TEST_ASSERT_TRUE(inputs.btnTestLavage);
}

void test_manual_output_commands_update_manual_buttons() {
  InputsSnapshot inputs;

  SimulatorCommandResult result = applySimulatorCommand("tambour on", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_MANU_TAMBOUR, result.action);
  TEST_ASSERT_TRUE(inputs.btnManuTambour);

  result = applySimulatorCommand("tambour off", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_MANU_TAMBOUR, result.action);
  TEST_ASSERT_FALSE(inputs.btnManuTambour);

  result = applySimulatorCommand("rincage on", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_MANU_RINCAGE, result.action);
  TEST_ASSERT_TRUE(inputs.btnManuRincage);

  result = applySimulatorCommand("rincage off", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_MANU_RINCAGE, result.action);
  TEST_ASSERT_FALSE(inputs.btnManuRincage);
}

void test_temperature_commands_update_values_and_validity() {
  InputsSnapshot inputs;

  SimulatorCommandResult result = applySimulatorCommand("temp eau lost", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::TEMP_BASSIN_LOST, result.action);
  TEST_ASSERT_FALSE(inputs.tempBassinValid);

  result = applySimulatorCommand("temp eau 12.5", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_TEMP_BASSIN, result.action);
  TEST_ASSERT_TRUE(inputs.tempBassinValid);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, 12.5F, inputs.tempBassinC);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, 12.5F, result.temperatureC);

  result = applySimulatorCommand("temp local lost", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::TEMP_LOCAL_LOST, result.action);
  TEST_ASSERT_FALSE(inputs.tempLocalValid);

  result = applySimulatorCommand("temp local -1.5", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_TEMP_LOCAL, result.action);
  TEST_ASSERT_TRUE(inputs.tempLocalValid);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, -1.5F, inputs.tempLocalC);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, -1.5F, result.temperatureC);
}

void test_commands_are_trimmed_and_case_insensitive() {
  InputsSnapshot inputs;

  SimulatorCommandResult result = applySimulatorCommand("  LaVaGe ON  ", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_EP_LAVAGE, result.action);
  TEST_ASSERT_TRUE(inputs.epLavage);

  result = applySimulatorCommand("  TEMP LOCAL 18  ", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::SET_TEMP_LOCAL, result.action);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, 18.0F, inputs.tempLocalC);
}

void test_help_status_empty_and_unknown_commands_are_reported_without_mutating_inputs() {
  InputsSnapshot inputs;
  inputs.epLavage = true;
  inputs.modeAuto = false;
  inputs.modeMaintenance = true;
  inputs.tempBassinC = 21.0F;

  SimulatorCommandResult result = applySimulatorCommand("", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::NONE, result.action);

  result = applySimulatorCommand("help", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::HELP, result.action);

  result = applySimulatorCommand("status", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::STATUS, result.action);

  result = applySimulatorCommand("  JoUrNaL  ", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::JOURNAL, result.action);

  result = applySimulatorCommand("temp eau nope", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::UNKNOWN, result.action);

  result = applySimulatorCommand("does not exist", inputs);
  TEST_ASSERT_EQUAL(SimulatorCommandAction::UNKNOWN, result.action);

  TEST_ASSERT_TRUE(inputs.epLavage);
  TEST_ASSERT_FALSE(inputs.modeAuto);
  TEST_ASSERT_TRUE(inputs.modeMaintenance);
  TEST_ASSERT_FLOAT_WITHIN(0.001F, 21.0F, inputs.tempBassinC);
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  UNITY_BEGIN();
  RUN_TEST(test_lavage_and_critical_commands_update_level_inputs);
  RUN_TEST(test_capot_and_mode_commands_update_inputs);
  RUN_TEST(test_pulse_commands_set_one_shot_buttons);
  RUN_TEST(test_manual_output_commands_update_manual_buttons);
  RUN_TEST(test_temperature_commands_update_values_and_validity);
  RUN_TEST(test_commands_are_trimmed_and_case_insensitive);
  RUN_TEST(test_help_status_empty_and_unknown_commands_are_reported_without_mutating_inputs);
  return UNITY_END();
}
