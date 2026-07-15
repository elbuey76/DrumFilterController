#include "hal/Kc868A16Profile.h"

namespace {
Kc868A16HardwareProfile makeProfile(Kc868A16ProfileId id,
                                    const char* name,
                                    const char* revision,
                                    uint8_t input1,
                                    uint8_t input2,
                                    uint8_t output1,
                                    uint8_t output2) {
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

  return id == Kc868A16ProfileId::REV15_GREEN_CANDIDATE ? rev15 : standard;
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

