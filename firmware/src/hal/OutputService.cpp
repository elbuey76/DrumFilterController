#include "OutputService.h"

#if USE_KC868_IO
#include <Arduino.h>
#endif

void OutputService::begin() {
  allOff();
}

void OutputService::apply(const OutputsCommand& outputs) {
  lastRequested_ = outputs;
  lastApplied_ = kc868EffectiveOutputs(outputs, hardwareOutputsArmed(), hardwareIoHealthy_);
  lastHardwareRawOutputs_ = kc868MapOutputs(lastApplied_);
#if USE_KC868_IO
  if (!kc868Io_.writeOutputs(lastHardwareRawOutputs_)) {
    hardwareIoHealthy_ = false;
    lastApplied_ = OutputsCommand{};
    lastHardwareRawOutputs_ = kc868AllOutputsOff();
    kc868Io_.writeOutputs(lastHardwareRawOutputs_);
  }
#endif
}

void OutputService::allOff() {
  lastRequested_ = OutputsCommand{};
  lastApplied_ = OutputsCommand{};
  lastHardwareRawOutputs_ = kc868AllOutputsOff();
#if USE_KC868_IO
  kc868Io_.writeOutputs(lastHardwareRawOutputs_);
#endif
}

OutputsCommand OutputService::lastApplied() const {
  return lastApplied_;
}

OutputsCommand OutputService::lastRequested() const {
  return lastRequested_;
}

bool OutputService::hardwareOutputsArmed() const {
  return HARDWARE_OUTPUTS_ARMED != 0;
}

bool OutputService::hardwareIoHealthy() const {
  return hardwareIoHealthy_;
}

void OutputService::setHardwareIoHealthy(bool healthy) {
  hardwareIoHealthy_ = healthy;
}

Kc868DigitalOutputsRaw OutputService::lastHardwareRawOutputs() const {
  return lastHardwareRawOutputs_;
}

bool OutputService::pulseRelayForDiagnostics(uint8_t relayNumber, uint16_t pulseMs) {
  if (relayNumber < 1 || relayNumber > kKc868DigitalOutputCount || !hardwareOutputsArmed() || !hardwareIoHealthy_) {
    return false;
  }

  if (pulseMs > 1000) {
    pulseMs = 1000;
  }

  OutputsCommand pulse;
  switch (static_cast<Kc868OutputSignal>(relayNumber - 1)) {
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
  const bool onOk = kc868Io_.writeOutputs(kc868MapOutputs(pulse));
  delay(pulseMs);
  const bool offOk = kc868Io_.writeOutputs(kc868AllOutputsOff());
  if (!onOk || !offOk) {
    hardwareIoHealthy_ = false;
    return false;
  }
  return true;
#else
  (void)pulseMs;
  return false;
#endif
}
