#pragma once

#include "app/Types.h"
#include "hal/Kc868Mapping.h"

#ifndef USE_KC868_IO
#define USE_KC868_IO 0
#endif

#ifndef HARDWARE_OUTPUTS_ARMED
#define HARDWARE_OUTPUTS_ARMED 0
#endif

#ifndef KC868_DIAGNOSTIC_PULSES_ONLY
#define KC868_DIAGNOSTIC_PULSES_ONLY 0
#endif

#ifndef KC868_DIAGNOSTIC_PULSE_MAX_MS
#define KC868_DIAGNOSTIC_PULSE_MAX_MS 1000
#endif

#include "hal/Kc868Pcf8574Io.h"

class OutputService {
public:
  explicit OutputService(Kc868Pcf8574Io* kc868Io = nullptr);
  void begin();
  void apply(const OutputsCommand& outputs);
  void allOff();
  OutputsCommand lastApplied() const;
  OutputsCommand lastRequested() const;
  bool hardwareOutputsArmed() const;
  bool diagnosticPulsesPermitted() const;
  bool diagnosticPulsesOnlyBuild() const;
  bool hardwareArmRequested() const;
  bool hardwareIoHealthy() const;
  void setInputHardwareHealthy(bool healthy);
  bool hardwareProfileValidated() const;
  bool bootOffVerified() const;
  bool outputFaultLatched() const;
  bool outputBankHealthy(size_t bankIndex) const;
  bool outputBankFaultLatched(size_t bankIndex) const;
  const Kc868A16HardwareProfile& hardwareProfile() const;
  const char* hardwareDisarmReason() const;
  const char* diagnosticPulseDisarmReason() const;
  Kc868DigitalOutputsRaw lastHardwareRawOutputs() const;
  bool pulseOutputForDiagnostics(uint8_t outputNumber, uint16_t pulseMs);
  bool pulseRelayForDiagnostics(uint8_t relayNumber, uint16_t pulseMs);

private:
  OutputsCommand lastRequested_{};
  OutputsCommand lastApplied_{};
  Kc868DigitalOutputsRaw lastHardwareRawOutputs_{};
  bool inputHardwareHealthy_ = false;
#if USE_KC868_IO
  Kc868Pcf8574Io* kc868Io_ = nullptr;
#endif
};
