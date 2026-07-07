#include "SerialSimulator.h"

#include <stdlib.h>
#include <string.h>

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
      stream_->println(F("Commande trop longue, tampon réinitialisé."));
    }
  }
}

void SerialSimulator::handleCommand(const String& command, InputsSnapshot& inputs, const Controller& controller, const OutputsCommand& outputs) {
  if (stream_ == nullptr || command.length() == 0) {
    return;
  }

  if (command == F("help")) {
    printHelp();
  } else if (command == F("status")) {
    printStatus(inputs, controller, outputs);
  } else if (command == F("lavage on")) {
    inputs.epLavage = true;
    setBool("EP_LAVAGE", inputs.epLavage);
  } else if (command == F("lavage off")) {
    inputs.epLavage = false;
    setBool("EP_LAVAGE", inputs.epLavage);
  } else if (command == F("critique on")) {
    inputs.epCritique = true;
    setBool("EP_CRITIQUE", inputs.epCritique);
  } else if (command == F("critique off")) {
    inputs.epCritique = false;
    setBool("EP_CRITIQUE", inputs.epCritique);
  } else if (command == F("capot open")) {
    inputs.capotOuvert = true;
    setBool("CAPOT_OUVERT", inputs.capotOuvert);
  } else if (command == F("capot close")) {
    inputs.capotOuvert = false;
    setBool("CAPOT_OUVERT", inputs.capotOuvert);
  } else if (command == F("auto")) {
    inputs.modeAuto = true;
    inputs.modeMaintenance = false;
    stream_->println(F("Mode demandé: AUTO"));
  } else if (command == F("maintenance")) {
    inputs.modeAuto = false;
    inputs.modeMaintenance = true;
    stream_->println(F("Mode demandé: MAINTENANCE"));
  } else if (command == F("reset")) {
    inputs.btnReset = true;
    stream_->println(F("RESET impulsion simulée"));
  } else if (command == F("test")) {
    inputs.btnTestLavage = true;
    stream_->println(F("TEST_LAVAGE impulsion simulée"));
  } else if (command == F("tambour on")) {
    inputs.btnManuTambour = true;
    setBool("MANU_TAMBOUR", inputs.btnManuTambour);
  } else if (command == F("tambour off")) {
    inputs.btnManuTambour = false;
    setBool("MANU_TAMBOUR", inputs.btnManuTambour);
  } else if (command == F("rincage on")) {
    inputs.btnManuRincage = true;
    setBool("MANU_RINCAGE", inputs.btnManuRincage);
  } else if (command == F("rincage off")) {
    inputs.btnManuRincage = false;
    setBool("MANU_RINCAGE", inputs.btnManuRincage);
  } else if (command == F("temp eau lost")) {
    inputs.tempBassinValid = false;
    stream_->println(F("TEMP_BASSIN = LOST"));
  } else if (command == F("temp local lost")) {
    inputs.tempLocalValid = false;
    stream_->println(F("TEMP_LOCAL = LOST"));
  } else if (parseTemperature(command, "temp eau ", inputs.tempBassinC)) {
    inputs.tempBassinValid = true;
    stream_->print(F("TEMP_BASSIN = "));
    stream_->println(inputs.tempBassinC, 1);
  } else if (parseTemperature(command, "temp local ", inputs.tempLocalC)) {
    inputs.tempLocalValid = true;
    stream_->print(F("TEMP_LOCAL = "));
    stream_->println(inputs.tempLocalC, 1);
  } else {
    stream_->print(F("Commande inconnue: "));
    stream_->println(command);
    stream_->println(F("Tapez 'help' pour la liste des commandes."));
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
  stream_->println(F("  auto | maintenance"));
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

bool SerialSimulator::parseTemperature(const String& command, const char* prefix, float& target) const {
  if (!command.startsWith(prefix)) {
    return false;
  }

  const String value = command.substring(strlen(prefix));
  if (value.length() == 0) {
    return false;
  }

  char* end = nullptr;
  const float parsed = strtof(value.c_str(), &end);
  if (end == value.c_str() || *end != '\0') {
    return false;
  }

  target = parsed;
  return true;
}
