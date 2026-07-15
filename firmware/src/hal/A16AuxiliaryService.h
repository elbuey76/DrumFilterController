#pragma once

#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>

#include "hal/Kc868A16Profile.h"

struct A16RtcStatus {
  bool present = false;
  bool timeValid = false;
  bool lostPower = false;
  uint16_t year = 0;
  uint8_t month = 0;
  uint8_t day = 0;
  uint8_t hour = 0;
  uint8_t minute = 0;
  uint8_t second = 0;
};

class A16AuxiliaryService {
public:
  A16AuxiliaryService(TwoWire& wire, const Kc868A16HardwareProfile& profile);

  void begin();
  void poll(unsigned long nowMs);
  void scanI2c(Stream& stream);
  bool lcdPresent() const;
  const A16RtcStatus& rtcStatus() const;
  TwoWire& wire();

private:
  bool ping(uint8_t address);
  void refreshRtc();

  TwoWire& wire_;
  const Kc868A16HardwareProfile& profile_;
  RTC_DS3231 rtc_;
  A16RtcStatus rtcStatus_{};
  bool begun_ = false;
  bool lcdPresent_ = false;
  unsigned long lastRtcPollMs_ = 0;
};

