#pragma once

#include <Arduino.h>

#include "hal/Kc868Mapping.h"

#ifndef KC868_I2C_SDA
#define KC868_I2C_SDA 4
#endif

#ifndef KC868_I2C_SCL
#define KC868_I2C_SCL 5
#endif

struct Kc868Pcf8574AddressMap {
  uint8_t inputBanks[kKc868DigitalBankCount] = {0x20, 0x21, 0x22, 0x23};
  uint8_t outputBanks[kKc868DigitalBankCount] = {0x24, 0x25, 0x26, 0x27};
};

class Kc868Pcf8574Io {
public:
  void begin();
  Kc868DigitalInputsRaw readInputs();
  bool writeOutputs(const Kc868DigitalOutputsRaw& raw);
  bool writeAllOutputsOff();
  void scanI2c(Stream& stream);

private:
  bool writeByte(uint8_t address, uint8_t value);
  bool readByte(uint8_t address, uint8_t& value);
  bool ping(uint8_t address);

  Kc868Pcf8574AddressMap addresses_{};
  bool begun_ = false;
};
