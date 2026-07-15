#pragma once

#include <stdint.h>

class Kc868I2cBus {
public:
  virtual ~Kc868I2cBus() = default;

  virtual bool begin(uint8_t sda, uint8_t scl) = 0;
  virtual bool writeByte(uint8_t address, uint8_t value) = 0;
  virtual bool readByte(uint8_t address, uint8_t& value) = 0;
  virtual bool ping(uint8_t address) = 0;
};

