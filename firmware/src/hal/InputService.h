#pragma once

#include "app/Types.h"
#include "hal/Kc868Mapping.h"

#ifndef USE_SIM_INPUTS
#define USE_SIM_INPUTS 1
#endif

#ifndef USE_KC868_IO
#define USE_KC868_IO 0
#endif

#if USE_KC868_IO
#include <Arduino.h>

#include "hal/Kc868Pcf8574Io.h"
#endif

class InputService {
public:
  void begin();
  InputsSnapshot read(unsigned long nowMs);
  InputsSnapshot& simulatedInputs();
  const InputsSnapshot& lastInputs() const;
  bool hardwareIoHealthy() const;
  Kc868DigitalInputsRaw lastHardwareRawInputs() const;
#if USE_KC868_IO
  void scanI2c(Stream& stream);
#endif

private:
  InputsSnapshot simulatedInputs_{};
  InputsSnapshot lastInputs_{};
  Kc868DigitalInputsRaw lastHardwareRawInputs_{};
  bool hardwareIoHealthy_ = true;
#if USE_KC868_IO
  Kc868Pcf8574Io kc868Io_{};
#endif
};
