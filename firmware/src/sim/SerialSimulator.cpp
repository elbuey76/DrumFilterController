#include "SerialSimulator.h"

namespace {
String normalizedCommand(String command) {
  command.trim();
  command.toLowerCase();
  return command;
}

const char* boolText(bool value) {
  return value ? "ON" : "OFF";
}
}  // namespace

void SerialSimulator::begin(Stream& stream) {
  stream_ = &stream;
  bufferLength_ = 0;
  printHelp();
}

void SerialSimulator::poll(InputsSnapshot& inputs, const Controller& controller, const OutputsCommand& outputs) {
  if (stream_ == nullptr) {
    return;
  }

  while (stream_->available() > 0) {
    const char received = static_cast<char>(stream_->read());

    if (received == '\r') {
      continue;
    }

    if (received == '\n') {
      buffer_[bufferLength_] = '\0';
      handleCommand(normalizedCommand(String(buffer_)), inputs, controller, outputs);
      bufferLength_ = 0;
      continue;
    }

    if (bufferLength_ < kBufferSize - 1) {
      buffer_[bufferLength_++] = received;
    } else {
      bufferLength_ = 0;
      stream_->println(F("Commande trop longue, tampon reinitialise."));
    }
  }
}

void SerialSimulator::handleCommand(const String& command, InputsSnapshot& inputs, const Controller& controller, const OutputsCommand& outputs) {
  if (stream_ == nullptr || command.length() == 0) {
    return;
  }

  const SimulatorCommandResult result = applySimulatorCommand(command.c_str(), inputs);

  switch (result.action) {
    case SimulatorCommandAction::NONE:
      break;
    case SimulatorCommandAction::HELP:
      printHelp();
      break;
    case SimulatorCommandAction::STATUS:
      printStatus(inputs, controller, outputs);
      break;
    case SimulatorCommandAction::SET_EP_LAVAGE:
      setBool("EP_LAVAGE", result.boolValue);
      break;
    case SimulatorCommandAction::SET_EP_CRITIQUE:
      setBool("EP_CRITIQUE", result.boolValue);
      break;
    case SimulatorCommandAction::SET_CAPOT_OUVERT:
      setBool("CAPOT_OUVERT", result.boolValue);
      break;
    case SimulatorCommandAction::MODE_AUTO:
      stream_->println(F("Mode demande: AUTO"));
      break;
    case SimulatorCommandAction::MODE_MAINTENANCE:
      stream_->println(F("Mode demande: MAINTENANCE"));
      break;
    case SimulatorCommandAction::MODE_MANUAL:
      stream_->println(F("Mode demande: MANUEL"));
      break;
    case SimulatorCommandAction::PULSE_RESET:
      stream_->println(F("RESET impulsion simulee"));
      break;
    case SimulatorCommandAction::PULSE_TEST_LAVAGE:
      stream_->println(F("TEST_LAVAGE impulsion simulee"));
      break;
    case SimulatorCommandAction::SET_MANU_TAMBOUR:
      setBool("MANU_TAMBOUR", result.boolValue);
      break;
    case SimulatorCommandAction::SET_MANU_RINCAGE:
      setBool("MANU_RINCAGE", result.boolValue);
      break;
    case SimulatorCommandAction::TEMP_BASSIN_LOST:
      stream_->println(F("TEMP_BASSIN = LOST"));
      break;
    case SimulatorCommandAction::TEMP_LOCAL_LOST:
      stream_->println(F("TEMP_LOCAL = LOST"));
      break;
    case SimulatorCommandAction::SET_TEMP_BASSIN:
      stream_->print(F("TEMP_BASSIN = "));
      stream_->println(result.temperatureC, 1);
      break;
    case SimulatorCommandAction::SET_TEMP_LOCAL:
      stream_->print(F("TEMP_LOCAL = "));
      stream_->println(result.temperatureC, 1);
      break;
    case SimulatorCommandAction::UNKNOWN:
      stream_->print(F("Commande inconnue: "));
      stream_->println(command);
      stream_->println(F("Tapez 'help' pour la liste des commandes."));
      break;
  }
}

void SerialSimulator::printHelp() const {
  if (stream_ == nullptr) {
    return;
  }

  stream_->println(F("Commandes simulateur:"));
  stream_->println(F("  help | status"));
  stream_->println(F("  lavage on|off"));
  stream_->println(F("  critique on|off"));
  stream_->println(F("  capot open|close"));
  stream_->println(F("  auto | maintenance | manual"));
  stream_->println(F("  reset | test"));
  stream_->println(F("  tambour on|off"));
  stream_->println(F("  rincage on|off"));
  stream_->println(F("  temp eau <degC> | temp local <degC>"));
  stream_->println(F("  temp eau lost | temp local lost"));
}

void SerialSimulator::printStatus(const InputsSnapshot& inputs, const Controller& controller, const OutputsCommand& outputs) const {
  if (stream_ == nullptr) {
    return;
  }

  const ControllerStatus status = controller.status();
  stream_->println(F("--- STATUS ---"));
  stream_->print(F("State: "));
  printState(status.state);
  stream_->print(F("Message: "));
  stream_->println(status.message);
  stream_->print(F("Alarm: "));
  stream_->println(status.alarmCode == nullptr ? "none" : status.alarmCode);
  printInputs(inputs);
  printOutputs(outputs);
}

void SerialSimulator::printInputs(const InputsSnapshot& inputs) const {
  stream_->print(F("EP_LAVAGE: "));
  stream_->println(boolText(inputs.epLavage));
  stream_->print(F("EP_CRITIQUE: "));
  stream_->println(boolText(inputs.epCritique));
  stream_->print(F("CAPOT_OUVERT: "));
  stream_->println(boolText(inputs.capotOuvert));
  stream_->print(F("MODE_AUTO: "));
  stream_->println(boolText(inputs.modeAuto));
  stream_->print(F("MODE_MAINTENANCE: "));
  stream_->println(boolText(inputs.modeMaintenance));
  stream_->print(F("TEMP_BASSIN: "));
  if (inputs.tempBassinValid) {
    stream_->println(inputs.tempBassinC, 1);
  } else {
    stream_->println(F("LOST"));
  }
  stream_->print(F("TEMP_LOCAL: "));
  if (inputs.tempLocalValid) {
    stream_->println(inputs.tempLocalC, 1);
  } else {
    stream_->println(F("LOST"));
  }
}

void SerialSimulator::printOutputs(const OutputsCommand& outputs) const {
  stream_->print(F("Tambour: "));
  stream_->println(boolText(outputs.cmdTambour));
  stream_->print(F("Rincage: "));
  stream_->println(boolText(outputs.cmdRincage));
  stream_->print(F("Filtration: "));
  stream_->println(boolText(outputs.cmdPompeFiltration));
  stream_->print(F("Deco: "));
  stream_->println(boolText(outputs.cmdPompeDeco));
  stream_->print(F("UV: "));
  stream_->println(boolText(outputs.cmdUv));
  stream_->print(F("Mise a niveau: "));
  stream_->println(boolText(outputs.cmdMiseANiveau));
  stream_->print(F("Voyant marche: "));
  stream_->println(boolText(outputs.voyantMarche));
  stream_->print(F("Voyant lavage: "));
  stream_->println(boolText(outputs.voyantLavage));
  stream_->print(F("Voyant alarme: "));
  stream_->println(boolText(outputs.voyantAlarme));
}

void SerialSimulator::printState(SystemState state) const {
  switch (state) {
    case SystemState::BOOT: stream_->println(F("BOOT")); break;
    case SystemState::AUTO_WAIT: stream_->println(F("AUTO_WAIT")); break;
    case SystemState::WASH_AUTO: stream_->println(F("WASH_AUTO")); break;
    case SystemState::POST_WASH: stream_->println(F("POST_WASH")); break;
    case SystemState::SAFETY_PAUSE: stream_->println(F("SAFETY_PAUSE")); break;
    case SystemState::RETRY_PAUSE: stream_->println(F("RETRY_PAUSE")); break;
    case SystemState::MANUAL: stream_->println(F("MANUAL")); break;
    case SystemState::MAINTENANCE: stream_->println(F("MAINTENANCE")); break;
    case SystemState::TEST_WASH: stream_->println(F("TEST_WASH")); break;
    case SystemState::DEGRADED: stream_->println(F("DEGRADED")); break;
    case SystemState::FAULT: stream_->println(F("FAULT")); break;
  }
}

void SerialSimulator::setBool(const char* label, bool value) const {
  if (stream_ == nullptr) {
    return;
  }

  stream_->print(label);
  stream_->print(F(" = "));
  stream_->println(boolText(value));
}
