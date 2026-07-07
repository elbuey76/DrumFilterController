#pragma once

#include "app/Types.h"
#include "hal/Kc868Mapping.h"

#ifndef USE_KC868_IO
#define USE_KC868_IO 0
#endif

#ifndef HARDWARE_OUTPUTS_ARMED
#define HARDWARE_OUTPUTS_ARMED 0
#endif

#if USE_KC868_IO
#include "hal/Kc868Pcf8574Io.h"
#endif

class OutputService {
public:
  void begin();
  void apply(const OutputsCommand& outputs);
  void allOff();
  OutputsCommand lastApplied() const;
  OutputsCommand lastRequested() const;
  bool hardwareOutputsArmed() const;
  bool hardwareIoHealthy() const;
  void setHardwareIoHealthy(bool healthy);
  Kc868DigitalOutputsRaw lastHardwareRawOutputs() const;
  bool pulseRelayForDiagnostics(uint8_t relayNumber, uint16_t pulseMs);

private:
  OutputsCommand lastRequested_{};
  OutputsCommand lastApplied_{};
  Kc868DigitalOutputsRaw lastHardwareRawOutputs_{};
  bool hardwareIoHealthy_ = true;
#if USE_KC868_IO
  Kc868Pcf8574Io kc868Io_{};
#endif
};
