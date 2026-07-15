#include "hal/A16AuxiliaryService.h"

A16AuxiliaryService::A16AuxiliaryService(TwoWire& wire, const Kc868A16HardwareProfile& profile)
    : wire_(wire), profile_(profile) {}

void A16AuxiliaryService::begin() {
  if (begun_) {
    return;
  }
  begun_ = wire_.begin(profile_.auxiliaries.i2cSda, profile_.auxiliaries.i2cScl);
  if (!begun_) {
    return;
  }

  lcdPresent_ = ping(profile_.auxiliaries.lcdAddress);
  rtcStatus_.present = ping(profile_.auxiliaries.rtcAddress) && rtc_.begin(&wire_);
  if (rtcStatus_.present) {
    refreshRtc();
  }
}

void A16AuxiliaryService::poll(unsigned long nowMs) {
  begin();
  if (!rtcStatus_.present || nowMs - lastRtcPollMs_ < 1000UL) {
    return;
  }
  lastRtcPollMs_ = nowMs;
  refreshRtc();
}

void A16AuxiliaryService::scanI2c(Stream& stream) {
  begin();
  stream.println(F("--- I2C AUXILIAIRE A16 ---"));
  uint8_t count = 0;
  for (uint8_t address = 1; address < 127; ++address) {
    if (!ping(address)) {
      continue;
    }
    stream.print(F("0x"));
    if (address < 16) {
      stream.print('0');
    }
    stream.println(address, HEX);
    ++count;
  }
  stream.print(F("Peripheriques detectes: "));
  stream.println(count);
}

bool A16AuxiliaryService::lcdPresent() const {
  return lcdPresent_;
}

const A16RtcStatus& A16AuxiliaryService::rtcStatus() const {
  return rtcStatus_;
}

TwoWire& A16AuxiliaryService::wire() {
  return wire_;
}

bool A16AuxiliaryService::ping(uint8_t address) {
  if (!begun_) {
    return false;
  }
  wire_.beginTransmission(address);
  return wire_.endTransmission() == 0;
}

void A16AuxiliaryService::refreshRtc() {
  rtcStatus_.lostPower = rtc_.lostPower();
  const DateTime now = rtc_.now();
  rtcStatus_.timeValid = !rtcStatus_.lostPower && now.isValid();
  rtcStatus_.year = now.year();
  rtcStatus_.month = now.month();
  rtcStatus_.day = now.day();
  rtcStatus_.hour = now.hour();
  rtcStatus_.minute = now.minute();
  rtcStatus_.second = now.second();
}

