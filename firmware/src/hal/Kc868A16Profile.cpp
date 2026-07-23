#include "hal/Kc868A16Profile.h"

namespace {
Kc868A16HardwareProfile makeProfile(Kc868A16ProfileId id,
                                    const char* name,
                                    const char* revision,
                                    uint8_t input1,
                                    uint8_t input2,
                                    uint8_t output1,
                                    uint8_t output2,
                                    bool diagnosticPulsesValidated = false) {
  Kc868A16HardwareProfile profile{};
  profile.id = id;
  profile.name = name;
  profile.pcbRevision = revision;
  profile.internalI2cSda = 4;
  profile.internalI2cScl = 5;
  profile.addresses.inputBanks[0] = input1;
  profile.addresses.inputBanks[1] = input2;
  profile.addresses.outputBanks[0] = output1;
  profile.addresses.outputBanks[1] = output2;
  profile.mapping = defaultKc868DigitalMapping();
  profile.auxiliaries.i2cSda = 32;
  profile.auxiliaries.i2cScl = 33;
  profile.auxiliaries.lcdAddress = 0x27;
  profile.auxiliaries.rtcAddress = 0x68;
  profile.auxiliaries.oneWirePin = 14;
  // Candidate profiles stay deliberately unvalidated until the received
  // board passes the physical mapping and all-outputs-OFF checks.
  profile.validated = false;
  profile.diagnosticPulsesValidated = diagnosticPulsesValidated;
  return profile;
}
}  // namespace

const Kc868A16HardwareProfile& kc868A16Profile(Kc868A16ProfileId id) {
  static const Kc868A16HardwareProfile standard =
      makeProfile(Kc868A16ProfileId::GENERIC_STANDARD_CANDIDATE,
                  "a16-classic-standard-candidate",
                  "unknown",
                  0x21,
                  0x22,
                  0x24,
                  0x25);
  static const Kc868A16HardwareProfile rev15 =
      makeProfile(Kc868A16ProfileId::REV15_GREEN_CANDIDATE,
                  "a16-rev1.5-green-candidate",
                  "REV1.5 green label (unverified)",
                  0x39,
                  0x3A,
                  0x3C,
                  0x3D);

  static const Kc868A16HardwareProfile rev163 = [] {
    Kc868A16HardwareProfile profile =
        makeProfile(Kc868A16ProfileId::REV163_INPUTS_CONFIRMED_CANDIDATE,
                    "a16-rev1.6.3-inputs-confirmed-candidate",
                    "REV.1.6.3 (I/O and DS18B20 ROMs confirmed; loads/auxiliaries pending)",
                    0x22,
                    0x21,
                    0x24,
                    0x25,
                    true);
    const uint8_t bassinRom[8] = {0x28, 0xA3, 0xAF, 0xC8, 0x00, 0x00, 0x00, 0x62};
    const uint8_t localRom[8] = {0x28, 0x4C, 0x19, 0xCC, 0x00, 0x00, 0x00, 0xB2};
    for (size_t index = 0; index < 8; ++index) {
      profile.auxiliaries.tempBassinRom[index] = bassinRom[index];
      profile.auxiliaries.tempLocalRom[index] = localRom[index];
    }
    return profile;
  }();

  switch (id) {
    case Kc868A16ProfileId::REV15_GREEN_CANDIDATE:
      return rev15;
    case Kc868A16ProfileId::REV163_INPUTS_CONFIRMED_CANDIDATE:
      return rev163;
    case Kc868A16ProfileId::GENERIC_STANDARD_CANDIDATE:
    default:
      return standard;
  }
}

const Kc868A16HardwareProfile& selectedKc868A16Profile() {
  return kc868A16Profile(static_cast<Kc868A16ProfileId>(KC868_A16_PROFILE_ID));
}

bool kc868RomConfigured(const uint8_t rom[8]) {
  for (size_t index = 0; index < 8; ++index) {
    if (rom[index] != 0) {
      return true;
    }
  }
  return false;
}
