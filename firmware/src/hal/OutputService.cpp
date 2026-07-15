#include "OutputService.h"

#if USE_KC868_IO
#include <Arduino.h>
#endif

OutputService::OutputService(Kc868Pcf8574Io* kc868Io) {
#if USE_KC868_IO
  kc868Io_ = kc868Io;
#else
  (void)kc868Io;
#endif
}

void OutputService::begin() {
#if USE_KC868_IO
  if (kc868Io_ != nullptr) {
    kc868Io_->begin();
  }
#endif
  allOff();
}

void OutputService::apply(const OutputsCommand& outputs) {
  lastRequested_ = outputs;
  Kc868HardwareSafetyState safety;
  safety.armRequested = hardwareArmRequested();
  safety.profileValidated = hardwareProfileValidated();
  safety.bootOffVerified = bootOffVerified();
  safety.inputBanksHealthy = inputHardwareHealthy_;
#if USE_KC868_IO
  safety.outputBanksHealthy = kc868Io_ != nullptr && kc868Io_->outputBanksHealthy();
#endif
  lastApplied_ = kc868EffectiveOutputs(outputs, safety);
#if USE_KC868_IO
  const Kc868DigitalMapping& mapping = kc868Io_ == nullptr ? defaultKc868DigitalMapping() : kc868Io_->profile().mapping;
#else
  const Kc868DigitalMapping& mapping = defaultKc868DigitalMapping();
#endif
  lastHardwareRawOutputs_ = kc868MapOutputs(lastApplied_, mapping);
#if USE_KC868_IO
  if (kc868Io_ == nullptr || !kc868Io_->writeOutputs(lastHardwareRawOutputs_)) {
    lastApplied_ = OutputsCommand{};
    lastHardwareRawOutputs_ = kc868AllOutputsOff(mapping);
    if (kc868Io_ != nullptr) {
      kc868Io_->writeAllOutputsOff();
    }
  }
#endif
}

void OutputService::allOff() {
  lastRequested_ = OutputsCommand{};
  lastApplied_ = OutputsCommand{};
#if USE_KC868_IO
  const Kc868DigitalMapping& mapping = kc868Io_ == nullptr ? defaultKc868DigitalMapping() : kc868Io_->profile().mapping;
#else
  const Kc868DigitalMapping& mapping = defaultKc868DigitalMapping();
#endif
  lastHardwareRawOutputs_ = kc868AllOutputsOff(mapping);
#if USE_KC868_IO
  if (kc868Io_ != nullptr) {
    kc868Io_->writeAllOutputsOff();
  }
#endif
}

OutputsCommand OutputService::lastApplied() const {
  return lastApplied_;
}

OutputsCommand OutputService::lastRequested() const {
  return lastRequested_;
}

bool OutputService::hardwareOutputsArmed() const {
  Kc868HardwareSafetyState safety;
  safety.armRequested = hardwareArmRequested();
  safety.profileValidated = hardwareProfileValidated();
  safety.bootOffVerified = bootOffVerified();
  safety.inputBanksHealthy = inputHardwareHealthy_;
#if USE_KC868_IO
  safety.outputBanksHealthy = kc868Io_ != nullptr && kc868Io_->outputBanksHealthy();
#endif
  return kc868HardwareOutputsArmed(safety);
}

bool OutputService::hardwareArmRequested() const {
  return HARDWARE_OUTPUTS_ARMED != 0;
}

bool OutputService::hardwareIoHealthy() const {
  if (!inputHardwareHealthy_) {
    return false;
  }
#if USE_KC868_IO
  return kc868Io_ != nullptr && kc868Io_->outputBanksHealthy() && !kc868Io_->outputFaultLatched();
#else
  return false;
#endif
}

void OutputService::setInputHardwareHealthy(bool healthy) {
  inputHardwareHealthy_ = healthy;
}

bool OutputService::hardwareProfileValidated() const {
#if USE_KC868_IO
  return kc868Io_ != nullptr && kc868Io_->profile().validated;
#else
  return false;
#endif
}

bool OutputService::bootOffVerified() const {
#if USE_KC868_IO
  return kc868Io_ != nullptr && kc868Io_->bootOffVerified();
#else
  return false;
#endif
}

bool OutputService::outputFaultLatched() const {
#if USE_KC868_IO
  return kc868Io_ == nullptr || kc868Io_->outputFaultLatched();
#else
  return false;
#endif
}

bool OutputService::outputBankHealthy(size_t bankIndex) const {
#if USE_KC868_IO
  return kc868Io_ != nullptr && bankIndex < kKc868DigitalOutputBankCount && kc868Io_->lastOutputBankOk()[bankIndex];
#else
  (void)bankIndex;
  return false;
#endif
}

bool OutputService::outputBankFaultLatched(size_t bankIndex) const {
#if USE_KC868_IO
  return kc868Io_ != nullptr && bankIndex < kKc868DigitalOutputBankCount && kc868Io_->latchedOutputBankFaults()[bankIndex];
#else
  (void)bankIndex;
  return false;
#endif
}

const Kc868A16HardwareProfile& OutputService::hardwareProfile() const {
#if USE_KC868_IO
  return kc868Io_ == nullptr ? selectedKc868A16Profile() : kc868Io_->profile();
#else
  return selectedKc868A16Profile();
#endif
}

const char* OutputService::hardwareDisarmReason() const {
  if (!hardwareArmRequested()) return "build safe: HARDWARE_OUTPUTS_ARMED=0";
  if (!hardwareProfileValidated()) return "profil A16 non valide";
  if (!bootOffVerified()) return "boot toutes sorties OFF non verifie";
  if (!inputHardwareHealthy_) return "banque d'entrees absente ou en defaut";
#if USE_KC868_IO
  if (kc868Io_ == nullptr) return "pilote A16 absent";
  if (outputBankFaultLatched(0)) return "defaut banque sorties O1-O8 verrouille";
  if (outputBankFaultLatched(1)) return "defaut banque sorties O9-O16 verrouille";
  if (kc868Io_->outputFaultLatched()) return "defaut bus sorties verrouille";
  if (!kc868Io_->outputBanksHealthy()) return "banque de sorties absente ou en defaut";
#endif
  return "arme";
}

Kc868DigitalOutputsRaw OutputService::lastHardwareRawOutputs() const {
  return lastHardwareRawOutputs_;
}

bool OutputService::pulseOutputForDiagnostics(uint8_t outputNumber, uint16_t pulseMs) {
  if (outputNumber < 1 || outputNumber > kKc868DigitalOutputCount || !hardwareOutputsArmed()) {
    return false;
  }

  if (pulseMs > 1000) {
    pulseMs = 1000;
  }

  OutputsCommand pulse;
  switch (static_cast<Kc868OutputSignal>(outputNumber - 1)) {
    case Kc868OutputSignal::CMD_TAMBOUR: pulse.cmdTambour = true; break;
    case Kc868OutputSignal::CMD_RINCAGE: pulse.cmdRincage = true; break;
    case Kc868OutputSignal::CMD_POMPE_FILTRATION: pulse.cmdPompeFiltration = true; break;
    case Kc868OutputSignal::CMD_POMPE_DECO: pulse.cmdPompeDeco = true; break;
    case Kc868OutputSignal::CMD_UV: pulse.cmdUv = true; break;
    case Kc868OutputSignal::CMD_MISE_A_NIVEAU: pulse.cmdMiseANiveau = true; break;
    case Kc868OutputSignal::VOYANT_MARCHE: pulse.voyantMarche = true; break;
    case Kc868OutputSignal::VOYANT_LAVAGE: pulse.voyantLavage = true; break;
    case Kc868OutputSignal::VOYANT_ALARME: pulse.voyantAlarme = true; break;
  }

#if USE_KC868_IO
  if (kc868Io_ == nullptr) {
    return false;
  }
  const bool onOk = kc868Io_->writeOutputs(kc868MapOutputs(pulse, kc868Io_->profile().mapping));
  delay(pulseMs);
  const bool offOk = kc868Io_->writeAllOutputsOff();
  if (!onOk || !offOk) {
    return false;
  }
  return true;
#else
  (void)pulseMs;
  return false;
#endif
}

bool OutputService::pulseRelayForDiagnostics(uint8_t relayNumber, uint16_t pulseMs) {
  return pulseOutputForDiagnostics(relayNumber, pulseMs);
}
