#pragma once

#include <stddef.h>
#include <stdint.h>

#include "hal/Kc868A16Profile.h"
#include "hal/Kc868I2cBus.h"

class Kc868Pcf8574Io {
public:
  Kc868Pcf8574Io(Kc868I2cBus& bus, const Kc868A16HardwareProfile& profile);

  bool begin();
  Kc868DigitalInputsRaw readInputs();
  bool writeOutputs(const Kc868DigitalOutputsRaw& raw);
  bool writeAllOutputsOff();
  size_t scanI2c(uint8_t* foundAddresses, size_t capacity);

  const Kc868A16HardwareProfile& profile() const;
  bool bootOffVerified() const;
  bool outputBanksHealthy() const;
  bool outputFaultLatched() const;
  const bool* lastOutputBankOk() const;
  const bool* latchedOutputBankFaults() const;

private:
  bool writeAllOutputsOffInternal();

  Kc868I2cBus& bus_;
  const Kc868A16HardwareProfile& profile_;
  bool begun_ = false;
  bool bootOffVerified_ = false;
  bool outputFaultLatched_ = false;
  bool outputBankOk_[kKc868DigitalOutputBankCount] = {false, false};
  bool outputBankFaultLatched_[kKc868DigitalOutputBankCount] = {false, false};
};
