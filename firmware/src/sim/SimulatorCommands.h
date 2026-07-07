#pragma once

#include "app/Types.h"

enum class SimulatorCommandAction {
  NONE,
  HELP,
  STATUS,
  SET_EP_LAVAGE,
  SET_EP_CRITIQUE,
  SET_CAPOT_OUVERT,
  MODE_AUTO,
  MODE_MAINTENANCE,
  MODE_MANUAL,
  PULSE_RESET,
  PULSE_TEST_LAVAGE,
  SET_MANU_TAMBOUR,
  SET_MANU_RINCAGE,
  TEMP_BASSIN_LOST,
  TEMP_LOCAL_LOST,
  SET_TEMP_BASSIN,
  SET_TEMP_LOCAL,
  UNKNOWN
};

struct SimulatorCommandResult {
  SimulatorCommandAction action = SimulatorCommandAction::NONE;
  bool boolValue = false;
  float temperatureC = 0.0F;
};

SimulatorCommandResult applySimulatorCommand(const char* command, InputsSnapshot& inputs);
