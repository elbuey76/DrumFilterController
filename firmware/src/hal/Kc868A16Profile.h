#pragma once

#include <stddef.h>
#include <stdint.h>

#include "hal/Kc868Mapping.h"

#ifndef KC868_A16_PROFILE_ID
#define KC868_A16_PROFILE_ID 0
#endif

enum class Kc868A16ProfileId : uint8_t {
  GENERIC_STANDARD_CANDIDATE = 0,
  REV15_GREEN_CANDIDATE = 1,
  REV163_INPUTS_CONFIRMED_CANDIDATE = 2,
};

struct Kc868Pcf8574AddressMap {
  uint8_t inputBanks[kKc868DigitalInputBankCount];
  uint8_t outputBanks[kKc868DigitalOutputBankCount];
};

struct Kc868A16AuxiliaryProfile {
  uint8_t i2cSda;
  uint8_t i2cScl;
  uint8_t lcdAddress;
  uint8_t rtcAddress;
  uint8_t oneWirePin;
  uint8_t tempBassinRom[8];
  uint8_t tempLocalRom[8];
};

struct Kc868A16HardwareProfile {
  Kc868A16ProfileId id;
  const char* name;
  const char* pcbRevision;
  uint8_t internalI2cSda;
  uint8_t internalI2cScl;
  Kc868Pcf8574AddressMap addresses;
  Kc868DigitalMapping mapping;
  Kc868A16AuxiliaryProfile auxiliaries;
  bool validated;
  bool diagnosticPulsesValidated;
};

const Kc868A16HardwareProfile& kc868A16Profile(Kc868A16ProfileId id);
const Kc868A16HardwareProfile& selectedKc868A16Profile();
bool kc868RomConfigured(const uint8_t rom[8]);
