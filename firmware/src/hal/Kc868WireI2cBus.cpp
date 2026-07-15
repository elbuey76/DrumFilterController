#include "hal/Kc868WireI2cBus.h"

Kc868WireI2cBus::Kc868WireI2cBus(TwoWire& wire) : wire_(wire) {}

bool Kc868WireI2cBus::begin(uint8_t sda, uint8_t scl) {
  if (!begun_) {
    begun_ = wire_.begin(sda, scl);
  }
  return begun_;
}

bool Kc868WireI2cBus::writeByte(uint8_t address, uint8_t value) {
  wire_.beginTransmission(address);
  wire_.write(value);
  return wire_.endTransmission() == 0;
}

bool Kc868WireI2cBus::readByte(uint8_t address, uint8_t& value) {
  const size_t received = wire_.requestFrom(static_cast<int>(address), 1);
  if (received != 1 || wire_.available() != 1) {
    return false;
  }
  value = static_cast<uint8_t>(wire_.read());
  return true;
}

bool Kc868WireI2cBus::ping(uint8_t address) {
  wire_.beginTransmission(address);
  return wire_.endTransmission() == 0;
}

