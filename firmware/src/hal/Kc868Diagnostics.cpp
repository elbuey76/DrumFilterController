#include "Kc868Diagnostics.h"

#if USE_A16_AUXILIARIES
#include "hal/A16AuxiliaryService.h"
#endif

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

void Kc868Diagnostics::begin(Stream& stream, A16AuxiliaryService* auxiliaryService) {
  stream_ = &stream;
  auxiliaryService_ = auxiliaryService;
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
  } else if (command == "diag aux i2c") {
#if USE_A16_AUXILIARIES
    if (auxiliaryService_ != nullptr) {
      auxiliaryService_->scanI2c(*stream_);
    } else {
      stream_->println(F("Bus auxiliaire indisponible."));
    }
#else
    stream_->println(F("Auxiliaires desactives dans ce build."));
#endif
  } else if (command == "diag rtc") {
    printRtc();
  } else if (command == "diag temp") {
#if USE_A16_AUXILIARIES
    inputService.printTemperatureDiagnostics(*stream_);
#else
    stream_->println(F("DS18B20 desactives dans ce build."));
#endif
  } else if (command.startsWith("diag output ") || command.startsWith("diag relay ")) {
    uint8_t outputNumber = 0;
    uint16_t pulseMs = 0;
    if (!parseOutputPulse(command, outputNumber, pulseMs)) {
      stream_->println(F("Syntaxe: diag output <1-9> pulse <ms>"));
      return;
    }

    if (!outputService.diagnosticPulsesPermitted()) {
      stream_->print(F("Refus: "));
      stream_->println(outputService.diagnosticPulseDisarmReason());
      return;
    }

    if (!outputService.pulseOutputForDiagnostics(outputNumber, pulseMs)) {
      stream_->println(F("Impulsion sortie echouee ou refusee."));
      return;
    }

    stream_->print(F("Sortie O"));
    stream_->print(outputNumber);
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
  stream_->println(F("  diag aux i2c | diag rtc | diag temp"));
  stream_->print(F("  diag output <1-9> pulse <ms> (max "));
  stream_->print(KC868_DIAGNOSTIC_PULSE_MAX_MS);
  stream_->println(F(" ms)"));
  stream_->println(F("  diag relay ... (alias historique)"));
}

void Kc868Diagnostics::printInputs(InputService& inputService, const InputsSnapshot& inputs) const {
  const Kc868DigitalInputsRaw raw = inputService.lastHardwareRawInputs();
  stream_->println(F("--- INPUTS ---"));
#if USE_KC868_IO
  const Kc868A16HardwareProfile& profile = inputService.hardwareProfile();
  stream_->print(F("Profil: "));
  stream_->println(profile.name);
  stream_->print(F("Revision attendue: "));
  stream_->println(profile.pcbRevision);
  stream_->print(F("I2C interne SDA/SCL: "));
  stream_->print(profile.internalI2cSda);
  stream_->print('/');
  stream_->println(profile.internalI2cScl);
#endif
  stream_->print(F("Hardware IO: "));
  stream_->println(inputService.hardwareIoHealthy() ? F("OK") : F("FAULT"));
  printRawBanks("Raw", raw.banks, kKc868DigitalInputBankCount);

  for (uint8_t index = 0; index < kKc868DigitalInputBankCount; ++index) {
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
  stream_->print(F("Profil: "));
  stream_->println(outputService.hardwareProfile().name);
  stream_->print(F("Revision attendue: "));
  stream_->println(outputService.hardwareProfile().pcbRevision);
  stream_->print(F("Armement demande par build: "));
  stream_->println(outputService.hardwareArmRequested() ? F("YES") : F("NO"));
  stream_->print(F("Profil materiel valide: "));
  stream_->println(outputService.hardwareProfileValidated() ? F("YES") : F("NO"));
  stream_->print(F("Boot toutes sorties OFF: "));
  stream_->println(outputService.bootOffVerified() ? F("OK") : F("FAULT"));
  stream_->print(F("Hardware outputs effectively armed: "));
  stream_->println(outputService.hardwareOutputsArmed() ? F("YES") : F("NO"));
  stream_->print(F("Impulsions diagnostic seulement: "));
  stream_->println(outputService.diagnosticPulsesOnlyBuild() ? F("YES") : F("NO"));
  stream_->print(F("Impulsions diagnostic autorisees: "));
  stream_->println(outputService.diagnosticPulsesPermitted() ? F("YES") : F("NO"));
  stream_->print(F("Etat armement: "));
  stream_->println(outputService.hardwareDisarmReason());
  stream_->print(F("Hardware IO: "));
  stream_->println(outputService.hardwareIoHealthy() ? F("OK") : F("FAULT"));
  stream_->print(F("Defaut sortie verrouille: "));
  stream_->println(outputService.outputFaultLatched() ? F("YES") : F("NO"));
  for (size_t index = 0; index < kKc868DigitalOutputBankCount; ++index) {
    stream_->print(F("Bank sorties "));
    stream_->print(index + 1);
    stream_->print(F(": "));
    stream_->print(outputService.outputBankHealthy(index) ? F("OK") : F("FAULT"));
    stream_->print(F("; defaut memorise: "));
    stream_->println(outputService.outputBankFaultLatched(index) ? F("YES") : F("NO"));
  }

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
  printRawBanks("Raw sorties", raw.banks, kKc868DigitalOutputBankCount);
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

void Kc868Diagnostics::printRawBanks(const char* label, const uint8_t* banks, size_t bankCount) const {
  stream_->print(label);
  stream_->print(F(":"));
  for (size_t index = 0; index < bankCount; ++index) {
    stream_->print(F(" 0x"));
    if (banks[index] < 16) {
      stream_->print('0');
    }
    stream_->print(banks[index], HEX);
  }
  stream_->println();
}

bool Kc868Diagnostics::parseOutputPulse(const String& command, uint8_t& outputNumber, uint16_t& pulseMs) const {
  const int firstSpace = command.indexOf(' ');
  const int secondSpace = command.indexOf(' ', firstSpace + 1);
  const int thirdSpace = command.indexOf(' ', secondSpace + 1);
  const int fourthSpace = command.indexOf(' ', thirdSpace + 1);
  if (firstSpace < 0 || secondSpace < 0 || thirdSpace < 0 || fourthSpace < 0) {
    return false;
  }

  const String target = command.substring(firstSpace + 1, secondSpace);
  if ((target != "output" && target != "relay") || command.substring(thirdSpace + 1, fourthSpace) != "pulse") {
    return false;
  }

  const int relay = command.substring(secondSpace + 1, thirdSpace).toInt();
  const int ms = command.substring(fourthSpace + 1).toInt();
  if (relay < 1 || relay > static_cast<int>(kKc868DigitalOutputCount) || ms <= 0) {
    return false;
  }

  outputNumber = static_cast<uint8_t>(relay);
  pulseMs = static_cast<uint16_t>(ms > KC868_DIAGNOSTIC_PULSE_MAX_MS ? KC868_DIAGNOSTIC_PULSE_MAX_MS : ms);
  return true;
}

void Kc868Diagnostics::printRtc() const {
  if (stream_ == nullptr) {
    return;
  }
#if USE_A16_AUXILIARIES
  if (auxiliaryService_ == nullptr) {
    stream_->println(F("RTC indisponible."));
    return;
  }
  const A16RtcStatus& rtc = auxiliaryService_->rtcStatus();
  stream_->println(F("--- RTC DS3231 ---"));
  stream_->print(F("Presente: "));
  stream_->println(rtc.present ? F("YES") : F("NO"));
  stream_->print(F("Heure valide: "));
  stream_->println(rtc.timeValid ? F("YES") : F("NO"));
  stream_->print(F("Perte alimentation detectee: "));
  stream_->println(rtc.lostPower ? F("YES") : F("NO"));
  if (rtc.present) {
    stream_->print(rtc.year);
    stream_->print('-');
    if (rtc.month < 10) stream_->print('0');
    stream_->print(rtc.month);
    stream_->print('-');
    if (rtc.day < 10) stream_->print('0');
    stream_->print(rtc.day);
    stream_->print(' ');
    if (rtc.hour < 10) stream_->print('0');
    stream_->print(rtc.hour);
    stream_->print(':');
    if (rtc.minute < 10) stream_->print('0');
    stream_->print(rtc.minute);
    stream_->print(':');
    if (rtc.second < 10) stream_->print('0');
    stream_->println(rtc.second);
  }
#else
  stream_->println(F("RTC desactivee dans ce build."));
#endif
}
