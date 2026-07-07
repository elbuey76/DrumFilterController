#include "Kc868Diagnostics.h"

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

void Kc868Diagnostics::begin(Stream& stream) {
  stream_ = &stream;
  lineBuffer_.reset();
  printHelp();
}

void Kc868Diagnostics::poll(InputService& inputService, OutputService& outputService, const InputsSnapshot& inputs, const Controller& controller) {
  if (stream_ == nullptr) {
    return;
  }

  while (stream_->available() > 0) {
    const char received = static_cast<char>(stream_->read());

    switch (lineBuffer_.push(received)) {
      case SimulatorLineEvent::NONE:
        break;
      case SimulatorLineEvent::LINE_READY:
        handleCommand(normalizedCommand(String(lineBuffer_.line())), inputService, outputService, inputs, controller);
        lineBuffer_.reset();
        break;
      case SimulatorLineEvent::BUFFER_OVERFLOW:
        stream_->println(F("Commande trop longue, tampon reinitialise."));
        lineBuffer_.reset();
        break;
    }
  }
}

void Kc868Diagnostics::handleCommand(const String& command, InputService& inputService, OutputService& outputService, const InputsSnapshot& inputs, const Controller& controller) {
  if (stream_ == nullptr || command.length() == 0) {
    return;
  }

  if (command == "help" || command == "diag help") {
    printHelp();
  } else if (command == "status") {
    printController(inputs, controller);
    printOutputs(outputService);
  } else if (command == "diag i2c") {
#if USE_KC868_IO
    inputService.scanI2c(*stream_);
#else
    stream_->println(F("I2C indisponible: USE_KC868_IO=0"));
#endif
  } else if (command == "diag inputs") {
    printInputs(inputService, inputs);
  } else if (command == "diag outputs") {
    printOutputs(outputService);
  } else if (command.startsWith("diag relay ")) {
    uint8_t relayNumber = 0;
    uint16_t pulseMs = 0;
    if (!parseRelayPulse(command, relayNumber, pulseMs)) {
      stream_->println(F("Syntaxe: diag relay <1-9> pulse <ms>"));
      return;
    }

    if (!outputService.hardwareOutputsArmed()) {
      stream_->println(F("Refus: HARDWARE_OUTPUTS_ARMED=0"));
      return;
    }

    if (!outputService.pulseRelayForDiagnostics(relayNumber, pulseMs)) {
      stream_->println(F("Impulsion relais echouee ou refusee."));
      return;
    }

    stream_->print(F("Relais R"));
    stream_->print(relayNumber);
    stream_->print(F(" pulse "));
    stream_->print(pulseMs);
    stream_->println(F(" ms OK"));
  } else {
    stream_->print(F("Commande inconnue: "));
    stream_->println(command);
    stream_->println(F("Tapez 'diag help' pour la liste des commandes."));
  }
}

void Kc868Diagnostics::printHelp() const {
  if (stream_ == nullptr) {
    return;
  }

  stream_->println(F("Commandes diagnostic KC868:"));
  stream_->println(F("  help | status"));
  stream_->println(F("  diag i2c"));
  stream_->println(F("  diag inputs"));
  stream_->println(F("  diag outputs"));
  stream_->println(F("  diag relay <1-9> pulse <ms>"));
}

void Kc868Diagnostics::printInputs(InputService& inputService, const InputsSnapshot& inputs) const {
  const Kc868DigitalInputsRaw raw = inputService.lastHardwareRawInputs();
  stream_->println(F("--- INPUTS ---"));
  stream_->print(F("Hardware IO: "));
  stream_->println(inputService.hardwareIoHealthy() ? F("OK") : F("FAULT"));
  printRawBanks("Raw", raw.banks);

  for (uint8_t index = 0; index < kKc868DigitalBankCount; ++index) {
    stream_->print(F("Bank "));
    stream_->print(index + 1);
    stream_->print(F(": "));
    stream_->println(raw.bankOk[index] ? F("OK") : F("FAULT"));
  }

  printBool("EP_LAVAGE", inputs.epLavage);
  printBool("EP_CRITIQUE", inputs.epCritique);
  printBool("CAPOT_OUVERT", inputs.capotOuvert);
  printBool("MODE_AUTO", inputs.modeAuto);
  printBool("MODE_MAINTENANCE", inputs.modeMaintenance);
  printBool("RESET_ALARME", inputs.btnReset);
  printBool("TEST_LAVAGE", inputs.btnTestLavage);
  printBool("MANU_TAMBOUR", inputs.btnManuTambour);
  printBool("MANU_RINCAGE", inputs.btnManuRincage);
}

void Kc868Diagnostics::printOutputs(const OutputService& outputService) const {
  stream_->println(F("--- OUTPUTS ---"));
  stream_->print(F("Hardware outputs armed: "));
  stream_->println(outputService.hardwareOutputsArmed() ? F("YES") : F("NO"));
  stream_->print(F("Hardware IO: "));
  stream_->println(outputService.hardwareIoHealthy() ? F("OK") : F("FAULT"));

  const OutputsCommand requested = outputService.lastRequested();
  const OutputsCommand applied = outputService.lastApplied();
  stream_->println(F("Demandes controleur:"));
  printBool("CMD_TAMBOUR", requested.cmdTambour);
  printBool("CMD_RINCAGE", requested.cmdRincage);
  printBool("CMD_POMPE_FILTRATION", requested.cmdPompeFiltration);
  printBool("CMD_POMPE_DECO", requested.cmdPompeDeco);
  printBool("CMD_UV", requested.cmdUv);
  printBool("CMD_MISE_A_NIVEAU", requested.cmdMiseANiveau);
  printBool("VOYANT_MARCHE", requested.voyantMarche);
  printBool("VOYANT_LAVAGE", requested.voyantLavage);
  printBool("VOYANT_ALARME", requested.voyantAlarme);

  stream_->println(F("Sorties appliquees:"));
  printBool("CMD_TAMBOUR", applied.cmdTambour);
  printBool("CMD_RINCAGE", applied.cmdRincage);
  printBool("CMD_POMPE_FILTRATION", applied.cmdPompeFiltration);
  printBool("CMD_POMPE_DECO", applied.cmdPompeDeco);
  printBool("CMD_UV", applied.cmdUv);
  printBool("CMD_MISE_A_NIVEAU", applied.cmdMiseANiveau);
  printBool("VOYANT_MARCHE", applied.voyantMarche);
  printBool("VOYANT_LAVAGE", applied.voyantLavage);
  printBool("VOYANT_ALARME", applied.voyantAlarme);

  const Kc868DigitalOutputsRaw raw = outputService.lastHardwareRawOutputs();
  printRawBanks("Raw sorties", raw.banks);
}

void Kc868Diagnostics::printController(const InputsSnapshot& inputs, const Controller& controller) const {
  const ControllerStatus status = controller.status();
  stream_->println(F("--- STATUS ---"));
  stream_->print(F("Message: "));
  stream_->println(status.message);
  stream_->print(F("Alarm: "));
  stream_->println(status.alarmCode == nullptr ? "none" : status.alarmCode);
  printBool("EP_LAVAGE", inputs.epLavage);
  printBool("EP_CRITIQUE", inputs.epCritique);
  printBool("CAPOT_OUVERT", inputs.capotOuvert);
}

void Kc868Diagnostics::printBool(const char* label, bool value) const {
  stream_->print(label);
  stream_->print(F(": "));
  stream_->println(boolText(value));
}

void Kc868Diagnostics::printRawBanks(const char* label, const uint8_t* banks) const {
  stream_->print(label);
  stream_->print(F(":"));
  for (uint8_t index = 0; index < kKc868DigitalBankCount; ++index) {
    stream_->print(F(" 0x"));
    if (banks[index] < 16) {
      stream_->print('0');
    }
    stream_->print(banks[index], HEX);
  }
  stream_->println();
}

bool Kc868Diagnostics::parseRelayPulse(const String& command, uint8_t& relayNumber, uint16_t& pulseMs) const {
  const int firstSpace = command.indexOf(' ');
  const int secondSpace = command.indexOf(' ', firstSpace + 1);
  const int thirdSpace = command.indexOf(' ', secondSpace + 1);
  const int fourthSpace = command.indexOf(' ', thirdSpace + 1);
  if (firstSpace < 0 || secondSpace < 0 || thirdSpace < 0 || fourthSpace < 0) {
    return false;
  }

  if (command.substring(firstSpace + 1, secondSpace) != "relay" || command.substring(thirdSpace + 1, fourthSpace) != "pulse") {
    return false;
  }

  const int relay = command.substring(secondSpace + 1, thirdSpace).toInt();
  const int ms = command.substring(fourthSpace + 1).toInt();
  if (relay < 1 || relay > static_cast<int>(kKc868DigitalOutputCount) || ms <= 0) {
    return false;
  }

  relayNumber = static_cast<uint8_t>(relay);
  pulseMs = static_cast<uint16_t>(ms > 1000 ? 1000 : ms);
  return true;
}
