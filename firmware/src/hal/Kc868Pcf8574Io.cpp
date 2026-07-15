#include "hal/Kc868Pcf8574Io.h"

Kc868Pcf8574Io::Kc868Pcf8574Io(Kc868I2cBus& bus, const Kc868A16HardwareProfile& profile)
    : bus_(bus), profile_(profile) {}

bool Kc868Pcf8574Io::begin() {
  if (begun_) {
    return bootOffVerified_;
  }

  begun_ = true;
  if (!bus_.begin(profile_.internalI2cSda, profile_.internalI2cScl)) {
    outputFaultLatched_ = true;
    return false;
  }

  // Output OFF is deliberately the first transaction on the internal bus.
  bootOffVerified_ = writeAllOutputsOffInternal();
  if (!bootOffVerified_) {
    outputFaultLatched_ = true;
  }

  // PCF8574 input pins must be released high before they can be read.
  for (size_t index = 0; index < kKc868DigitalInputBankCount; ++index) {
    bus_.writeByte(profile_.addresses.inputBanks[index], 0xFF);
  }
  return bootOffVerified_;
}
Kc868DigitalInputsRaw Kc868Pcf8574Io::readInputs() {
  begin();
  Kc868DigitalInputsRaw raw;
  for (size_t index = 0; index < kKc868DigitalInputBankCount; ++index) {
    raw.bankOk[index] = bus_.readByte(profile_.addresses.inputBanks[index], raw.banks[index]);
  }
  return raw;
}

bool Kc868Pcf8574Io::writeOutputs(const Kc868DigitalOutputsRaw& raw) {
  begin();
  if (outputFaultLatched_ || !bootOffVerified_) {
    return false;
  }

  bool ok = true;
  for (size_t index = 0; index < kKc868DigitalOutputBankCount; ++index) {
    outputBankOk_[index] = bus_.writeByte(profile_.addresses.outputBanks[index], raw.banks[index]);
    if (!outputBankOk_[index]) {
      outputBankFaultLatched_[index] = true;
    }
    ok = outputBankOk_[index] && ok;
  }

  if (!ok) {
    outputFaultLatched_ = true;
    writeAllOutputsOffInternal();
  }
  return ok;
}

bool Kc868Pcf8574Io::writeAllOutputsOff() {
  begin();
  const bool ok = writeAllOutputsOffInternal();
  if (!ok) {
    outputFaultLatched_ = true;
  }
  return ok;
}

size_t Kc868Pcf8574Io::scanI2c(uint8_t* foundAddresses, size_t capacity) {
  begin();
  size_t count = 0;
  for (uint8_t address = 1; address < 127; ++address) {
    if (bus_.ping(address)) {
      if (count < capacity) {
        foundAddresses[count] = address;
      }
      ++count;
    }
  }
  return count;
}

const Kc868A16HardwareProfile& Kc868Pcf8574Io::profile() const {
  return profile_;
}

bool Kc868Pcf8574Io::bootOffVerified() const {
  return bootOffVerified_;
}

bool Kc868Pcf8574Io::outputBanksHealthy() const {
  if (outputFaultLatched_) {
    return false;
  }
  for (size_t index = 0; index < kKc868DigitalOutputBankCount; ++index) {
    if (!outputBankOk_[index]) {
      return false;
    }
  }
  return true;
}

bool Kc868Pcf8574Io::outputFaultLatched() const {
  return outputFaultLatched_;
}

const bool* Kc868Pcf8574Io::lastOutputBankOk() const {
  return outputBankOk_;
}

const bool* Kc868Pcf8574Io::latchedOutputBankFaults() const {
  return outputBankFaultLatched_;
}

bool Kc868Pcf8574Io::writeAllOutputsOffInternal() {
  const Kc868DigitalOutputsRaw off = kc868AllOutputsOff(profile_.mapping);
  bool ok = true;
  for (size_t index = 0; index < kKc868DigitalOutputBankCount; ++index) {
    outputBankOk_[index] = bus_.writeByte(profile_.addresses.outputBanks[index], off.banks[index]);
    if (!outputBankOk_[index]) {
      outputBankFaultLatched_[index] = true;
    }
    ok = outputBankOk_[index] && ok;
  }
  return ok;
}
