#pragma once

#include "app/Types.h"
#include "hal/Kc868Mapping.h"

#ifndef USE_SIM_INPUTS
#define USE_SIM_INPUTS 1
#endif

#ifndef USE_KC868_IO
#define USE_KC868_IO 0
#endif

#ifndef USE_A16_AUXILIARIES
#define USE_A16_AUXILIARIES 0
#endif

#include <Arduino.h>
#include "hal/Kc868Pcf8574Io.h"

class TemperatureService;

class InputService {
public:
  explicit InputService(Kc868Pcf8574Io* kc868Io = nullptr, TemperatureService* temperatureService = nullptr);
  void begin();
  InputsSnapshot read(unsigned long nowMs);
  InputsSnapshot& simulatedInputs();
  const InputsSnapshot& lastInputs() const;
  bool hardwareIoHealthy() const;
  Kc868DigitalInputsRaw lastHardwareRawInputs() const;
#if USE_KC868_IO
  void scanI2c(Stream& stream);
  const Kc868A16HardwareProfile& hardwareProfile() const;
#endif
#if USE_A16_AUXILIARIES
  void printTemperatureDiagnostics(Stream& stream) const;
#endif

private:
  InputsSnapshot simulatedInputs_{};
  InputsSnapshot lastInputs_{};
  Kc868DigitalInputsRaw lastHardwareRawInputs_{};
  bool hardwareIoHealthy_ = true;
#if USE_KC868_IO
  Kc868Pcf8574Io* kc868Io_ = nullptr;
#endif
#if USE_A16_AUXILIARIES
  TemperatureService* temperatureService_ = nullptr;
#endif
};
