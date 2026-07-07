#include "SimulatorCommands.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

namespace {
constexpr size_t kCommandBufferSize = 96;

bool equals(const char* lhs, const char* rhs) {
  return strcmp(lhs, rhs) == 0;
}

bool startsWith(const char* value, const char* prefix) {
  return strncmp(value, prefix, strlen(prefix)) == 0;
}

void normalizeCommand(const char* command, char* normalized, size_t normalizedSize) {
  if (normalizedSize == 0) {
    return;
  }

  size_t start = 0;
  while (command[start] != '\0' && isspace(static_cast<unsigned char>(command[start])) != 0) {
    ++start;
  }

  size_t end = strlen(command);
  while (end > start && isspace(static_cast<unsigned char>(command[end - 1])) != 0) {
    --end;
  }

  size_t out = 0;
  for (size_t i = start; i < end && out < normalizedSize - 1; ++i) {
    normalized[out++] = static_cast<char>(tolower(static_cast<unsigned char>(command[i])));
  }
  normalized[out] = '\0';
}

bool parseTemperature(const char* command, const char* prefix, float& target) {
  if (!startsWith(command, prefix)) {
    return false;
  }

  const char* value = command + strlen(prefix);
  if (*value == '\0') {
    return false;
  }

  char* end = nullptr;
  const float parsed = strtof(value, &end);
  if (end == value || *end != '\0') {
    return false;
  }

  target = parsed;
  return true;
}

SimulatorCommandResult result(SimulatorCommandAction action, bool boolValue = false) {
  SimulatorCommandResult commandResult;
  commandResult.action = action;
  commandResult.boolValue = boolValue;
  return commandResult;
}

SimulatorCommandResult temperatureResult(SimulatorCommandAction action, float temperatureC) {
  SimulatorCommandResult commandResult;
  commandResult.action = action;
  commandResult.temperatureC = temperatureC;
  return commandResult;
}
}  // namespace

SimulatorCommandResult applySimulatorCommand(const char* command, InputsSnapshot& inputs) {
  char normalized[kCommandBufferSize]{};
  normalizeCommand(command == nullptr ? "" : command, normalized, sizeof(normalized));

  if (normalized[0] == '\0') {
    return result(SimulatorCommandAction::NONE);
  }

  if (equals(normalized, "help")) {
    return result(SimulatorCommandAction::HELP);
  }

  if (equals(normalized, "status")) {
    return result(SimulatorCommandAction::STATUS);
  }

  if (equals(normalized, "journal")) {
    return result(SimulatorCommandAction::JOURNAL);
  }

  if (equals(normalized, "lavage on")) {
    inputs.epLavage = true;
    return result(SimulatorCommandAction::SET_EP_LAVAGE, inputs.epLavage);
  }

  if (equals(normalized, "lavage off")) {
    inputs.epLavage = false;
    return result(SimulatorCommandAction::SET_EP_LAVAGE, inputs.epLavage);
  }

  if (equals(normalized, "critique on")) {
    inputs.epCritique = true;
    return result(SimulatorCommandAction::SET_EP_CRITIQUE, inputs.epCritique);
  }

  if (equals(normalized, "critique off")) {
    inputs.epCritique = false;
    return result(SimulatorCommandAction::SET_EP_CRITIQUE, inputs.epCritique);
  }

  if (equals(normalized, "capot open")) {
    inputs.capotOuvert = true;
    return result(SimulatorCommandAction::SET_CAPOT_OUVERT, inputs.capotOuvert);
  }

  if (equals(normalized, "capot close")) {
    inputs.capotOuvert = false;
    return result(SimulatorCommandAction::SET_CAPOT_OUVERT, inputs.capotOuvert);
  }

  if (equals(normalized, "auto")) {
    inputs.modeAuto = true;
    inputs.modeMaintenance = false;
    return result(SimulatorCommandAction::MODE_AUTO);
  }

  if (equals(normalized, "maintenance")) {
    inputs.modeAuto = false;
    inputs.modeMaintenance = true;
    return result(SimulatorCommandAction::MODE_MAINTENANCE);
  }

  if (equals(normalized, "manual")) {
    inputs.modeAuto = false;
    inputs.modeMaintenance = false;
    return result(SimulatorCommandAction::MODE_MANUAL);
  }

  if (equals(normalized, "reset")) {
    inputs.btnReset = true;
    return result(SimulatorCommandAction::PULSE_RESET);
  }

  if (equals(normalized, "test")) {
    inputs.btnTestLavage = true;
    return result(SimulatorCommandAction::PULSE_TEST_LAVAGE);
  }

  if (equals(normalized, "tambour on")) {
    inputs.btnManuTambour = true;
    return result(SimulatorCommandAction::SET_MANU_TAMBOUR, inputs.btnManuTambour);
  }

  if (equals(normalized, "tambour off")) {
    inputs.btnManuTambour = false;
    return result(SimulatorCommandAction::SET_MANU_TAMBOUR, inputs.btnManuTambour);
  }

  if (equals(normalized, "rincage on")) {
    inputs.btnManuRincage = true;
    return result(SimulatorCommandAction::SET_MANU_RINCAGE, inputs.btnManuRincage);
  }

  if (equals(normalized, "rincage off")) {
    inputs.btnManuRincage = false;
    return result(SimulatorCommandAction::SET_MANU_RINCAGE, inputs.btnManuRincage);
  }

  if (equals(normalized, "temp eau lost")) {
    inputs.tempBassinValid = false;
    return result(SimulatorCommandAction::TEMP_BASSIN_LOST);
  }

  if (equals(normalized, "temp local lost")) {
    inputs.tempLocalValid = false;
    return result(SimulatorCommandAction::TEMP_LOCAL_LOST);
  }

  float parsedTemperature = 0.0F;
  if (parseTemperature(normalized, "temp eau ", parsedTemperature)) {
    inputs.tempBassinValid = true;
    inputs.tempBassinC = parsedTemperature;
    return temperatureResult(SimulatorCommandAction::SET_TEMP_BASSIN, inputs.tempBassinC);
  }

  if (parseTemperature(normalized, "temp local ", parsedTemperature)) {
    inputs.tempLocalValid = true;
    inputs.tempLocalC = parsedTemperature;
    return temperatureResult(SimulatorCommandAction::SET_TEMP_LOCAL, inputs.tempLocalC);
  }

  return result(SimulatorCommandAction::UNKNOWN);
}
