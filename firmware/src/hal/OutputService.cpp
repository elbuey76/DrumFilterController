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
  const Kc868DigitalMapping& mapping =
#if USE_KC868_IO
      kc868Io_ == nullptr ? defaultKc868DigitalMapping() : kc868Io_->profile().mapping;
#else
      defaultKc868DigitalMapping();
#endif

#if KC868_DIAGNOSTIC_PULSES_ONLY
  // This build is exclusively for a human-triggered, bounded diagnostic pulse.
  // Never let the controller energize an output in it.
  lastApplied_ = OutputsCommand{};
  lastHardwareRawOutputs_ = kc868AllOutputsOff(mapping);
#if USE_KC868_IO
  if (kc868Io_ == nullptr || !kc868Io_->writeOutputs(lastHardwareRawOutputs_)) {
    if (kc868Io_ != nullptr) {
      kc868Io_->writeAllOutputsOff();
    }
  }
#endif
  return;
#endif

  Kc868HardwareSafetyState safety;
  safety.armRequested = hardwareArmRequested();
  safety.profileValidated = hardwareProfileValidated();
  safety.bootOffVerified = bootOffVerified();
  safety.inputBanksHealthy = inputHardwareHealthy_;
#if USE_KC868_IO
  safety.outputBanksHealthy = kc868Io_ != nullptr && kc868Io_->outputBanksHealthy();
#endif
  lastApplied_ = kc868EffectiveOutputs(outputs, safety);
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

bool OutputService::diagnosticPulsesPermitted() const {
  Kc868HardwareSafetyState safety;
  safety.armRequested = hardwareArmRequested();
  safety.profileValidated = hardwareProfileValidated();
  safety.bootOffVerified = bootOffVerified();
  safety.inputBanksHealthy = inputHardwareHealthy_;
#if USE_KC868_IO
  safety.outputBanksHealthy = kc868Io_ != nullptr && kc868Io_->outputBanksHealthy();
  const bool profileDiagnosticPulsesValidated = kc868Io_ != nullptr && kc868Io_->profile().diagnosticPulsesValidated;
#else
  safety.outputBanksHealthy = false;
  const bool profileDiagnosticPulsesValidated = false;
#endif
  return kc868DiagnosticPulsesPermitted(safety, profileDiagnosticPulsesValidated, diagnosticPulsesOnlyBuild());
}

bool OutputService::diagnosticPulsesOnlyBuild() const {
  return KC868_DIAGNOSTIC_PULSES_ONLY != 0;
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

const char* OutputService::diagnosticPulseDisarmReason() const {
  if (diagnosticPulsesPermitted()) return "autorise";
  if (!diagnosticPulsesOnlyBuild()) return hardwareDisarmReason();
#if USE_KC868_IO
  if (kc868Io_ == nullptr) return "pilote A16 absent";
  if (!kc868Io_->profile().diagnosticPulsesValidated) return "profil non autorise pour impulsions de diagnostic";
#endif
  if (!bootOffVerified()) return "boot toutes sorties OFF non verifie";
  if (!inputHardwareHealthy_) return "banque d'entrees absente ou en defaut";
  if (outputFaultLatched()) return "defaut sorties verrouille";
  return "banque de sorties absente ou en defaut";
}

Kc868DigitalOutputsRaw OutputService::lastHardwareRawOutputs() const {
  return lastHardwareRawOutputs_;
}

bool OutputService::pulseOutputForDiagnostics(uint8_t outputNumber, uint16_t pulseMs) {
  return pulsePhysicalOutputForDiagnostics(outputNumber, pulseMs);
}

bool OutputService::pulsePhysicalOutputForDiagnostics(uint8_t physicalOutputNumber, uint16_t pulseMs) {
  if (physicalOutputNumber < 1 || physicalOutputNumber > kKc868PhysicalOutputCount || !diagnosticPulsesPermitted()) {
    return false;
  }

  if (pulseMs > KC868_DIAGNOSTIC_PULSE_MAX_MS) {
    pulseMs = KC868_DIAGNOSTIC_PULSE_MAX_MS;
  }

#if USE_KC868_IO
  if (kc868Io_ == nullptr) {
    return false;
  }
  const Kc868DigitalOutputsRaw raw = kc868PhysicalOutputDiagnosticPulse(physicalOutputNumber, kc868Io_->profile().mapping);
  const bool onOk = kc868Io_->writeOutputs(raw);
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
