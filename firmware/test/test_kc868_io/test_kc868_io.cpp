#include <unity.h>

#include <string.h>

#include "hal/Kc868A16Profile.h"
#include "hal/Kc868I2cBus.h"
#include "hal/Kc868Pcf8574Io.h"

namespace {
struct WriteRecord {
  uint8_t address;
  uint8_t value;
};

class FakeI2cBus : public Kc868I2cBus {
public:
  bool begin(uint8_t sda, uint8_t scl) override {
    begun = beginOk;
    beginSda = sda;
    beginScl = scl;
    return beginOk;
  }

  bool writeByte(uint8_t address, uint8_t value) override {
    if (writeCount < sizeof(writes) / sizeof(writes[0])) {
      writes[writeCount] = WriteRecord{address, value};
    }
    ++writeCount;
    return reachable[address] && address != failingWriteAddress;
  }

  bool readByte(uint8_t address, uint8_t& value) override {
    if (!reachable[address]) {
      return false;
    }
    value = readValues[address];
    return true;
  }

  bool ping(uint8_t address) override {
    return reachable[address];
  }

  void makeProfileReachable(const Kc868A16HardwareProfile& profile) {
    for (size_t index = 0; index < kKc868DigitalInputBankCount; ++index) {
      reachable[profile.addresses.inputBanks[index]] = true;
      readValues[profile.addresses.inputBanks[index]] = 0xFF;
    }
    for (size_t index = 0; index < kKc868DigitalOutputBankCount; ++index) {
      reachable[profile.addresses.outputBanks[index]] = true;
    }
  }

  bool beginOk = true;
  bool begun = false;
  uint8_t beginSda = 0;
  uint8_t beginScl = 0;
  bool reachable[128]{};
  uint8_t readValues[128]{};
  WriteRecord writes[32]{};
  size_t writeCount = 0;
  uint8_t failingWriteAddress = 0;
};
}  // namespace

void setUp() {}
void tearDown() {}

void test_a16_candidate_profiles_are_explicit_and_never_validated() {
  const Kc868A16HardwareProfile& standard = kc868A16Profile(Kc868A16ProfileId::GENERIC_STANDARD_CANDIDATE);
  TEST_ASSERT_EQUAL_UINT8(4, standard.internalI2cSda);
  TEST_ASSERT_EQUAL_UINT8(5, standard.internalI2cScl);
  TEST_ASSERT_EQUAL_HEX8(0x21, standard.addresses.inputBanks[0]);
  TEST_ASSERT_EQUAL_HEX8(0x22, standard.addresses.inputBanks[1]);
  TEST_ASSERT_EQUAL_HEX8(0x24, standard.addresses.outputBanks[0]);
  TEST_ASSERT_EQUAL_HEX8(0x25, standard.addresses.outputBanks[1]);
  TEST_ASSERT_FALSE(standard.validated);

  const Kc868A16HardwareProfile& rev15 = kc868A16Profile(Kc868A16ProfileId::REV15_GREEN_CANDIDATE);
  TEST_ASSERT_EQUAL_HEX8(0x39, rev15.addresses.inputBanks[0]);
  TEST_ASSERT_EQUAL_HEX8(0x3A, rev15.addresses.inputBanks[1]);
  TEST_ASSERT_EQUAL_HEX8(0x3C, rev15.addresses.outputBanks[0]);
  TEST_ASSERT_EQUAL_HEX8(0x3D, rev15.addresses.outputBanks[1]);
  TEST_ASSERT_FALSE(rev15.validated);
}

void test_a16_boot_writes_both_output_banks_off_before_initializing_inputs() {
  const Kc868A16HardwareProfile& profile = selectedKc868A16Profile();
  FakeI2cBus bus;
  bus.makeProfileReachable(profile);
  Kc868Pcf8574Io io(bus, profile);

  TEST_ASSERT_TRUE(io.begin());
  TEST_ASSERT_TRUE(io.bootOffVerified());
  TEST_ASSERT_EQUAL_UINT8(4, bus.beginSda);
  TEST_ASSERT_EQUAL_UINT8(5, bus.beginScl);
  TEST_ASSERT_EQUAL_UINT32(4, bus.writeCount);
  TEST_ASSERT_EQUAL_HEX8(profile.addresses.outputBanks[0], bus.writes[0].address);
  TEST_ASSERT_EQUAL_HEX8(0xFF, bus.writes[0].value);
  TEST_ASSERT_EQUAL_HEX8(profile.addresses.outputBanks[1], bus.writes[1].address);
  TEST_ASSERT_EQUAL_HEX8(0xFF, bus.writes[1].value);
  TEST_ASSERT_EQUAL_HEX8(profile.addresses.inputBanks[0], bus.writes[2].address);
  TEST_ASSERT_EQUAL_HEX8(profile.addresses.inputBanks[1], bus.writes[3].address);
}

void test_a16_missing_input_bank_is_reported_without_changing_output_health() {
  const Kc868A16HardwareProfile& profile = selectedKc868A16Profile();
  FakeI2cBus bus;
  bus.makeProfileReachable(profile);
  bus.reachable[profile.addresses.inputBanks[1]] = false;
  Kc868Pcf8574Io io(bus, profile);
  io.begin();

  const Kc868DigitalInputsRaw raw = io.readInputs();
  TEST_ASSERT_TRUE(raw.bankOk[0]);
  TEST_ASSERT_FALSE(raw.bankOk[1]);
  TEST_ASSERT_TRUE(io.outputBanksHealthy());
}

void test_a16_output_write_failure_latches_fault_and_refuses_later_writes() {
  const Kc868A16HardwareProfile& profile = selectedKc868A16Profile();
  FakeI2cBus bus;
  bus.makeProfileReachable(profile);
  Kc868Pcf8574Io io(bus, profile);
  TEST_ASSERT_TRUE(io.begin());

  bus.failingWriteAddress = profile.addresses.outputBanks[1];
  OutputsCommand command;
  command.cmdTambour = true;
  TEST_ASSERT_FALSE(io.writeOutputs(kc868MapOutputs(command, profile.mapping)));
  TEST_ASSERT_TRUE(io.outputFaultLatched());
  TEST_ASSERT_FALSE(io.outputBanksHealthy());
  TEST_ASSERT_FALSE(io.latchedOutputBankFaults()[0]);
  TEST_ASSERT_TRUE(io.latchedOutputBankFaults()[1]);

  bus.failingWriteAddress = 0;
  const size_t writesBeforeRetry = bus.writeCount;
  TEST_ASSERT_FALSE(io.writeOutputs(kc868AllOutputsOff(profile.mapping)));
  TEST_ASSERT_EQUAL_UINT32(writesBeforeRetry, bus.writeCount);
}

void test_a16_scan_reports_devices_but_does_not_assign_roles() {
  const Kc868A16HardwareProfile& profile = selectedKc868A16Profile();
  FakeI2cBus bus;
  bus.makeProfileReachable(profile);
  bus.reachable[0x68] = true;
  Kc868Pcf8574Io io(bus, profile);
  uint8_t found[8]{};

  const size_t count = io.scanI2c(found, sizeof(found));
  TEST_ASSERT_EQUAL_UINT32(5, count);
  TEST_ASSERT_EQUAL_HEX8(profile.addresses.inputBanks[0], found[0]);
  TEST_ASSERT_EQUAL_HEX8(0x68, found[4]);
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;
  UNITY_BEGIN();
  RUN_TEST(test_a16_candidate_profiles_are_explicit_and_never_validated);
  RUN_TEST(test_a16_boot_writes_both_output_banks_off_before_initializing_inputs);
  RUN_TEST(test_a16_missing_input_bank_is_reported_without_changing_output_health);
  RUN_TEST(test_a16_output_write_failure_latches_fault_and_refuses_later_writes);
  RUN_TEST(test_a16_scan_reports_devices_but_does_not_assign_roles);
  return UNITY_END();
}
