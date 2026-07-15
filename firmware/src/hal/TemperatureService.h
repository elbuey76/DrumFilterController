#pragma once

#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#include "app/Types.h"
#include "hal/Kc868A16Profile.h"

class TemperatureService {
public:
  explicit TemperatureService(const Kc868A16HardwareProfile& profile);

  void begin();
  void poll(unsigned long nowMs);
  void applyTo(InputsSnapshot& inputs) const;
  void printDiagnostics(Stream& stream) const;

private:
  static constexpr size_t kMaxDiscoveredDevices = 8;
  static constexpr unsigned long kConversionTimeMs = 750UL;

  bool readConfigured(const uint8_t rom[8], float& valueC);
  void printRom(Stream& stream, const uint8_t rom[8]) const;

  const Kc868A16HardwareProfile& profile_;
  OneWire oneWire_;
  DallasTemperature sensors_;
  DeviceAddress discovered_[kMaxDiscoveredDevices]{};
  uint8_t discoveredCount_ = 0;
  bool begun_ = false;
  bool conversionPending_ = false;
  unsigned long conversionRequestedMs_ = 0;
  bool bassinValid_ = false;
  bool localValid_ = false;
  float bassinC_ = 0.0F;
  float localC_ = 0.0F;
};

