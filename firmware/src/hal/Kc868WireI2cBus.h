#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "hal/Kc868I2cBus.h"

class Kc868WireI2cBus : public Kc868I2cBus {
public:
  explicit Kc868WireI2cBus(TwoWire& wire = Wire);

  bool begin(uint8_t sda, uint8_t scl) override;
  bool writeByte(uint8_t address, uint8_t value) override;
  bool readByte(uint8_t address, uint8_t& value) override;
  bool ping(uint8_t address) override;

private:
  TwoWire& wire_;
  bool begun_ = false;
};

