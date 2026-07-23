#include "hal/TemperatureService.h"

#include <math.h>

TemperatureService::TemperatureService(const Kc868A16HardwareProfile& profile)
    : profile_(profile), oneWire_(profile.auxiliaries.oneWirePin), sensors_(&oneWire_) {}

void TemperatureService::begin() {
  if (begun_) {
    return;
  }
  begun_ = true;
  sensors_.begin();
  sensors_.setWaitForConversion(false);

  const uint8_t count = sensors_.getDeviceCount();
  discoveredCount_ = count < kMaxDiscoveredDevices ? count : kMaxDiscoveredDevices;
  for (uint8_t index = 0; index < discoveredCount_; ++index) {
    sensors_.getAddress(discovered_[index], index);
  }
  sensors_.requestTemperatures();
  conversionRequestedMs_ = millis();
  conversionPending_ = true;
}

void TemperatureService::poll(unsigned long nowMs) {
  begin();
  if (!conversionPending_ || nowMs - conversionRequestedMs_ < kConversionTimeMs) {
    return;
  }

  bassinValid_ = readConfigured(profile_.auxiliaries.tempBassinRom, bassinC_);
  localValid_ = readConfigured(profile_.auxiliaries.tempLocalRom, localC_);

  sensors_.requestTemperatures();
  conversionRequestedMs_ = nowMs;
  conversionPending_ = true;
}

void TemperatureService::applyTo(InputsSnapshot& inputs) const {
  inputs.tempBassinValid = bassinValid_;
  inputs.tempLocalValid = localValid_;
  if (bassinValid_) {
    inputs.tempBassinC = bassinC_;
  }
  if (localValid_) {
    inputs.tempLocalC = localC_;
  }
}

void TemperatureService::printDiagnostics(Stream& stream) const {
  stream.println(F("--- DS18B20 ---"));
  stream.print(F("GPIO 1-Wire: "));
  stream.println(profile_.auxiliaries.oneWirePin);
  stream.print(F("Sondes detectees: "));
  stream.println(discoveredCount_);
  for (uint8_t index = 0; index < discoveredCount_; ++index) {
    stream.print(F("  "));
    printRom(stream, discovered_[index]);
    stream.println();
  }
  stream.print(F("TEMP_BASSIN ROM: "));
  if (kc868RomConfigured(profile_.auxiliaries.tempBassinRom)) {
    printRom(stream, profile_.auxiliaries.tempBassinRom);
  } else {
    stream.print(F("NON CONFIGUREE"));
  }
  stream.println();
  stream.print(F("TEMP_LOCAL ROM: "));
  if (kc868RomConfigured(profile_.auxiliaries.tempLocalRom)) {
    printRom(stream, profile_.auxiliaries.tempLocalRom);
  } else {
    stream.print(F("NON CONFIGUREE"));
  }
  stream.println();
  stream.print(F("TEMP_BASSIN: "));
  if (bassinValid_) {
    stream.print(bassinC_, 2);
    stream.println(F(" deg C"));
  } else {
    stream.println(F("INVALIDE"));
  }
  stream.print(F("TEMP_LOCAL: "));
  if (localValid_) {
    stream.print(localC_, 2);
    stream.println(F(" deg C"));
  } else {
    stream.println(F("INVALIDE"));
  }
}

bool TemperatureService::readConfigured(const uint8_t rom[8], float& valueC) {
  if (!kc868RomConfigured(rom)) {
    return false;
  }
  DeviceAddress address{};
  for (size_t index = 0; index < 8; ++index) {
    address[index] = rom[index];
  }
  if (!sensors_.isConnected(address)) {
    return false;
  }
  const float value = sensors_.getTempC(address);
  if (!isfinite(value) || value == DEVICE_DISCONNECTED_C || value < -55.0F || value > 125.0F) {
    return false;
  }
  valueC = value;
  return true;
}

void TemperatureService::printRom(Stream& stream, const uint8_t rom[8]) const {
  for (size_t index = 0; index < 8; ++index) {
    if (rom[index] < 16) {
      stream.print('0');
    }
    stream.print(rom[index], HEX);
  }
}
