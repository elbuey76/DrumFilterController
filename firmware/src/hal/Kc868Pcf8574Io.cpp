#include "Kc868Pcf8574Io.h"

#include <Wire.h>

void Kc868Pcf8574Io::begin() {
  if (begun_) {
    return;
  }

  Wire.begin(KC868_I2C_SDA, KC868_I2C_SCL);
  begun_ = true;

  for (size_t index = 0; index < kKc868DigitalBankCount; ++index) {
    writeByte(addresses_.inputBanks[index], 0xFF);
  }
}

Kc868DigitalInputsRaw Kc868Pcf8574Io::readInputs() {
  begin();

  Kc868DigitalInputsRaw raw;
  for (size_t index = 0; index < kKc868DigitalBankCount; ++index) {
    raw.bankOk[index] = readByte(addresses_.inputBanks[index], raw.banks[index]);
  }
  return raw;
}

bool Kc868Pcf8574Io::writeOutputs(const Kc868DigitalOutputsRaw& raw) {
  begin();

  bool ok = true;
  for (size_t index = 0; index < kKc868DigitalBankCount; ++index) {
    ok = writeByte(addresses_.outputBanks[index], raw.banks[index]) && ok;
  }
  return ok;
}

bool Kc868Pcf8574Io::writeAllOutputsOff() {
  return writeOutputs(kc868AllOutputsOff());
}

void Kc868Pcf8574Io::scanI2c(Stream& stream) {
  begin();

  stream.println(F("--- I2C SCAN ---"));
  uint8_t count = 0;
  for (uint8_t address = 1; address < 127; ++address) {
    if (ping(address)) {
      stream.print(F("0x"));
      if (address < 16) {
        stream.print('0');
      }
      stream.println(address, HEX);
      ++count;
    }
  }
  stream.print(F("Peripheriques detectes: "));
  stream.println(count);
}

bool Kc868Pcf8574Io::writeByte(uint8_t address, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write(value);
  return Wire.endTransmission() == 0;
}

bool Kc868Pcf8574Io::readByte(uint8_t address, uint8_t& value) {
  Wire.requestFrom(static_cast<int>(address), 1);
  if (Wire.available() != 1) {
    return false;
  }

  value = static_cast<uint8_t>(Wire.read());
  return true;
}

bool Kc868Pcf8574Io::ping(uint8_t address) {
  Wire.beginTransmission(address);
  return Wire.endTransmission() == 0;
}
