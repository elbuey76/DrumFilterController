#include <unity.h>

#include "app/Controller.h"

namespace {

class FakePersistentStore : public PersistentStore {
public:
  void begin() override {
    ++beginCount;
  }

  PersistentJournalSnapshot load() override {
    return snapshot;
  }

  void save(const PersistentJournalSnapshot& nextSnapshot) override {
    snapshot = nextSnapshot;
    ++saveCount;
  }

  bool available() const override {
    return true;
  }

  uint32_t count(PersistentEventCode eventCode) const {
    return snapshot.eventCounts[persistentEventIndex(eventCode)];
  }

  PersistentJournalSnapshot snapshot;
  uint8_t beginCount = 0;
  uint8_t saveCount = 0;
};

class ControllerHarness {
public:
  Config config;
  InputsSnapshot inputs;
  OutputsCommand outputs;

  explicit ControllerHarness(PersistentStore* persistentStore = nullptr) : config(makeFastConfig()), controller_(config, persistentStore) {
    controller_.begin(nowMs_);
  }

  OutputsCommand tick() {
    outputs = controller_.update(inputs, nowMs_);
    return outputs;
  }

  OutputsCommand advance(unsigned long deltaMs) {
    nowMs_ += deltaMs;
    return tick();
  }

  OutputsCommand pressReset() {
    inputs.btnReset = true;
    OutputsCommand result = tick();
    inputs.btnReset = false;
    return result;
  }

  OutputsCommand pressTest() {
    inputs.btnTestLavage = true;
    OutputsCommand result = tick();
    inputs.btnTestLavage = false;
    return result;
  }

  OutputsCommand startAutoWash() {
    inputs.epLavage = true;
    tick();
    return advance(config.epLavageDelayMs);
  }

  ControllerStatus status() const {
    return controller_.status();
  }

  PersistentJournalSnapshot journalSnapshot() const {
    return controller_.journalSnapshot();
  }

private:
  static Config makeFastConfig() {
    Config cfg;
    cfg.epLavageDelayMs = 10;
    cfg.epCritiqueDebounceMs = 10;
    cfg.capotOpenDebounceMs = 10;
    cfg.capotCloseStableMs = 10;
    cfg.washMinMs = 10;
    cfg.washMaxMs = 30;
    cfg.residualRotationMs = 10;
    cfg.antiRestartMs = 10;
    cfg.retryPauseMs = 10;
    return cfg;
  }

  unsigned long nowMs_ = 0;
  Controller controller_;
};

void expectNoAlarm(const ControllerStatus& status) {
  TEST_ASSERT_NULL(status.alarmCode);
}

void expectAlarm(const ControllerStatus& status, const char* alarmCode) {
  TEST_ASSERT_EQUAL_STRING(alarmCode, status.alarmCode);
}

void expectDangerousOutputsOff(const OutputsCommand& outputs) {
  TEST_ASSERT_FALSE(outputs.cmdTambour);
  TEST_ASSERT_FALSE(outputs.cmdRincage);
}

void expectNominalFiltrationOn(const OutputsCommand& outputs) {
  TEST_ASSERT_TRUE(outputs.cmdPompeFiltration);
  TEST_ASSERT_TRUE(outputs.cmdUv);
}

void expectJournalCount(const FakePersistentStore& store, PersistentEventCode eventCode, uint32_t expectedCount) {
  TEST_ASSERT_EQUAL_UINT32(expectedCount, store.count(eventCode));
}

}  // namespace

void setUp() {}

void tearDown() {}

void test_sim_001_boot_nominal() {
  ControllerHarness h;

  h.tick();

  TEST_ASSERT_EQUAL(SystemState::AUTO_WAIT, h.status().state);
  TEST_ASSERT_EQUAL_STRING("AUTO - ATTENTE", h.status().message);
  expectNoAlarm(h.status());
  expectNominalFiltrationOn(h.outputs);
  TEST_ASSERT_FALSE(h.outputs.cmdTambour);
  TEST_ASSERT_FALSE(h.outputs.cmdRincage);
  TEST_ASSERT_FALSE(h.outputs.voyantAlarme);
}

void test_sim_002_capot_open_switches_to_maintenance() {
  ControllerHarness h;
  h.tick();

  h.inputs.capotOuvert = true;
  h.tick();
  h.advance(h.config.capotOpenDebounceMs);

  TEST_ASSERT_EQUAL(SystemState::MAINTENANCE, h.status().state);
  TEST_ASSERT_EQUAL_STRING("MAINTENANCE - CAPOT OUVERT", h.status().message);
  expectNoAlarm(h.status());
  expectNominalFiltrationOn(h.outputs);
  expectDangerousOutputsOff(h.outputs);
}

void test_sim_003_critical_level_incoherence_latches_until_reset() {
  ControllerHarness h;
  h.tick();

  h.inputs.epCritique = true;
  h.tick();
  h.advance(h.config.epCritiqueDebounceMs);

  TEST_ASSERT_EQUAL(SystemState::FAULT, h.status().state);
  expectAlarm(h.status(), "A02");
  expectDangerousOutputsOff(h.outputs);
  TEST_ASSERT_TRUE(h.outputs.voyantAlarme);

  h.inputs.epCritique = false;
  h.tick();
  h.advance(h.config.epCritiqueDebounceMs);

  TEST_ASSERT_EQUAL(SystemState::FAULT, h.status().state);
  expectAlarm(h.status(), "A02");

  h.pressReset();

  TEST_ASSERT_EQUAL(SystemState::AUTO_WAIT, h.status().state);
  expectNoAlarm(h.status());
}

void test_sim_004_005_auto_wash_nominal_then_returns_to_auto_wait() {
  ControllerHarness h;
  h.tick();

  h.startAutoWash();

  TEST_ASSERT_EQUAL(SystemState::WASH_AUTO, h.status().state);
  TEST_ASSERT_TRUE(h.outputs.cmdTambour);
  TEST_ASSERT_TRUE(h.outputs.cmdRincage);
  TEST_ASSERT_TRUE(h.outputs.voyantLavage);

  h.advance(h.config.washMinMs);
  h.inputs.epLavage = false;
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::POST_WASH, h.status().state);
  TEST_ASSERT_TRUE(h.outputs.cmdTambour);
  TEST_ASSERT_FALSE(h.outputs.cmdRincage);

  h.advance(h.config.residualRotationMs);

  TEST_ASSERT_EQUAL(SystemState::SAFETY_PAUSE, h.status().state);
  expectDangerousOutputsOff(h.outputs);

  h.advance(h.config.antiRestartMs);

  TEST_ASSERT_EQUAL(SystemState::AUTO_WAIT, h.status().state);
  TEST_ASSERT_EQUAL_STRING("AUTO - ATTENTE", h.status().message);
  expectNoAlarm(h.status());
  expectNominalFiltrationOn(h.outputs);
  expectDangerousOutputsOff(h.outputs);
}

void test_sim_006_persistent_wash_demand_retries_then_degrades_with_a04() {
  ControllerHarness h;
  h.tick();

  h.startAutoWash();

  TEST_ASSERT_EQUAL(SystemState::WASH_AUTO, h.status().state);

  h.advance(h.config.washMaxMs);

  TEST_ASSERT_EQUAL(SystemState::RETRY_PAUSE, h.status().state);
  expectNoAlarm(h.status());
  expectDangerousOutputsOff(h.outputs);

  h.advance(h.config.retryPauseMs);

  TEST_ASSERT_EQUAL(SystemState::WASH_AUTO, h.status().state);

  h.advance(h.config.washMaxMs);

  TEST_ASSERT_EQUAL(SystemState::RETRY_PAUSE, h.status().state);
  expectDangerousOutputsOff(h.outputs);

  h.advance(h.config.retryPauseMs);

  TEST_ASSERT_EQUAL(SystemState::WASH_AUTO, h.status().state);

  h.advance(h.config.washMaxMs);

  TEST_ASSERT_EQUAL(SystemState::DEGRADED, h.status().state);
  expectAlarm(h.status(), "A04");
  expectDangerousOutputsOff(h.outputs);
  TEST_ASSERT_TRUE(h.outputs.voyantAlarme);
}

void test_sim_007_capot_open_during_wash_triggers_a03_and_blocks_reset() {
  ControllerHarness h;
  h.tick();
  h.startAutoWash();

  h.inputs.capotOuvert = true;
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::FAULT, h.status().state);
  expectAlarm(h.status(), "A03");
  expectDangerousOutputsOff(h.outputs);
  TEST_ASSERT_TRUE(h.outputs.voyantAlarme);

  h.pressReset();

  TEST_ASSERT_EQUAL(SystemState::FAULT, h.status().state);
  expectAlarm(h.status(), "A05");
  expectDangerousOutputsOff(h.outputs);
}

void test_sim_008_capot_fault_can_reset_after_capot_close_stable() {
  ControllerHarness h;
  h.tick();
  h.startAutoWash();

  h.inputs.capotOuvert = true;
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::FAULT, h.status().state);
  expectAlarm(h.status(), "A03");

  h.inputs.capotOuvert = false;
  h.tick();
  h.advance(h.config.capotCloseStableMs);
  h.pressReset();

  TEST_ASSERT_EQUAL(SystemState::AUTO_WAIT, h.status().state);
  expectNoAlarm(h.status());
  expectNominalFiltrationOn(h.outputs);
  expectDangerousOutputsOff(h.outputs);
}

void test_sim_009_test_refused_when_capot_open() {
  ControllerHarness h;
  h.tick();

  h.inputs.capotOuvert = true;
  h.pressTest();

  TEST_ASSERT_EQUAL(SystemState::MAINTENANCE, h.status().state);
  expectAlarm(h.status(), "A13");
  expectDangerousOutputsOff(h.outputs);
  TEST_ASSERT_TRUE(h.outputs.voyantAlarme);
}

void test_sim_010_test_refused_when_level_safety_fault_active() {
  ControllerHarness h;
  h.tick();

  h.inputs.epCritique = true;
  h.tick();
  h.advance(h.config.epCritiqueDebounceMs);
  h.pressTest();

  TEST_ASSERT_EQUAL(SystemState::FAULT, h.status().state);
  expectAlarm(h.status(), "A14");
  expectDangerousOutputsOff(h.outputs);
  TEST_ASSERT_TRUE(h.outputs.voyantAlarme);
}

void test_sim_011_test_wash_ok_returns_success_message() {
  ControllerHarness h;
  h.tick();

  h.pressTest();

  TEST_ASSERT_EQUAL(SystemState::TEST_WASH, h.status().state);
  TEST_ASSERT_TRUE(h.outputs.cmdTambour);
  TEST_ASSERT_TRUE(h.outputs.cmdRincage);

  h.advance(h.config.washMinMs);

  TEST_ASSERT_EQUAL(SystemState::AUTO_WAIT, h.status().state);
  TEST_ASSERT_EQUAL_STRING("TEST OK - CYCLE EXECUTE", h.status().message);
  expectNoAlarm(h.status());
  expectDangerousOutputsOff(h.outputs);

  h.advance(h.config.residualRotationMs);

  TEST_ASSERT_EQUAL(SystemState::AUTO_WAIT, h.status().state);
  TEST_ASSERT_EQUAL_STRING("AUTO - ATTENTE", h.status().message);
  expectNoAlarm(h.status());
}

void test_sim_012_manual_tambour_command_runs_in_manual_mode() {
  ControllerHarness h;
  h.tick();

  h.inputs.modeAuto = false;
  h.inputs.modeMaintenance = true;
  h.inputs.btnManuTambour = true;
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::MANUAL, h.status().state);
  TEST_ASSERT_EQUAL_STRING("COMMANDE MANUELLE", h.status().message);
  expectNoAlarm(h.status());
  TEST_ASSERT_TRUE(h.outputs.cmdTambour);
  TEST_ASSERT_FALSE(h.outputs.cmdRincage);
  TEST_ASSERT_TRUE(h.outputs.voyantLavage);
}

void test_sim_013_capot_open_during_manual_command_triggers_a03() {
  ControllerHarness h;
  h.tick();

  h.inputs.modeAuto = false;
  h.inputs.modeMaintenance = true;
  h.inputs.btnManuTambour = true;
  h.tick();

  TEST_ASSERT_TRUE(h.outputs.cmdTambour);

  h.inputs.capotOuvert = true;
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::FAULT, h.status().state);
  expectAlarm(h.status(), "A03");
  expectDangerousOutputsOff(h.outputs);
}

void test_sim_014_missing_water_temperature_warns_without_stopping_nominal_outputs() {
  ControllerHarness h;
  h.tick();

  h.inputs.tempBassinValid = false;
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::AUTO_WAIT, h.status().state);
  expectAlarm(h.status(), "A11");
  expectNominalFiltrationOn(h.outputs);
  expectDangerousOutputsOff(h.outputs);
  TEST_ASSERT_TRUE(h.outputs.voyantAlarme);
}

void test_sim_015_missing_local_temperature_warns_without_stopping_nominal_outputs() {
  ControllerHarness h;
  h.tick();

  h.inputs.tempBassinValid = true;
  h.inputs.tempLocalValid = false;
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::AUTO_WAIT, h.status().state);
  expectAlarm(h.status(), "A12");
  expectNominalFiltrationOn(h.outputs);
  expectDangerousOutputsOff(h.outputs);
  TEST_ASSERT_TRUE(h.outputs.voyantAlarme);
}

void test_sim_016_failed_test_wash_does_not_latch_a04() {
  ControllerHarness h;
  h.tick();

  h.inputs.tempLocalC = 18.0F;
  h.inputs.epLavage = true;
  h.pressTest();

  TEST_ASSERT_EQUAL(SystemState::TEST_WASH, h.status().state);
  TEST_ASSERT_TRUE(h.outputs.cmdTambour);
  TEST_ASSERT_TRUE(h.outputs.cmdRincage);

  h.advance(h.config.washMinMs);

  TEST_ASSERT_EQUAL(SystemState::AUTO_WAIT, h.status().state);
  TEST_ASSERT_EQUAL_STRING("TEST ECHEC - LAVAGE INEFFICACE", h.status().message);
  expectNoAlarm(h.status());
  TEST_ASSERT_FALSE(h.outputs.voyantAlarme);
  expectDangerousOutputsOff(h.outputs);
}

void test_persist_001_boot_is_counted_on_each_begin() {
  FakePersistentStore store;

  {
    ControllerHarness h(&store);
    h.tick();
  }

  {
    ControllerHarness h(&store);
    h.tick();
  }

  expectJournalCount(store, PersistentEventCode::BOOT, 2);
  TEST_ASSERT_EQUAL_UINT32(2, store.snapshot.totalEvents);
  TEST_ASSERT_EQUAL(PersistentEventCode::BOOT, store.snapshot.lastEvent);
}

void test_persist_002_a15_active_reappears_after_reboot_without_double_counting() {
  FakePersistentStore store;
  store.snapshot.a15Active = true;
  store.snapshot.eventCounts[persistentEventIndex(PersistentEventCode::A15)] = 1;
  store.snapshot.totalEvents = 1;
  store.snapshot.lastEvent = PersistentEventCode::A15;

  ControllerHarness h(&store);
  h.inputs.capotOuvert = true;
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::MAINTENANCE, h.status().state);
  expectAlarm(h.status(), "A15");
  TEST_ASSERT_TRUE(h.outputs.voyantAlarme);
  TEST_ASSERT_TRUE(store.snapshot.a15Active);
  expectJournalCount(store, PersistentEventCode::A15, 1);
}

void test_persist_003_a15_active_is_cleared_after_reboot_with_closed_capot() {
  FakePersistentStore store;
  store.snapshot.a15Active = true;
  store.snapshot.eventCounts[persistentEventIndex(PersistentEventCode::A15)] = 1;
  store.snapshot.totalEvents = 1;
  store.snapshot.lastEvent = PersistentEventCode::A15;

  ControllerHarness h(&store);
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::AUTO_WAIT, h.status().state);
  expectNoAlarm(h.status());
  TEST_ASSERT_FALSE(store.snapshot.a15Active);
  expectJournalCount(store, PersistentEventCode::A15, 1);
}

void test_persist_004_open_capot_without_persisted_a15_restarts_long_open_timer() {
  FakePersistentStore store;
  ControllerHarness h(&store);

  h.inputs.capotOuvert = true;
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::MAINTENANCE, h.status().state);
  expectNoAlarm(h.status());
  TEST_ASSERT_FALSE(store.snapshot.a15Active);

  h.advance(h.config.capotLongOpenMs);

  TEST_ASSERT_EQUAL(SystemState::MAINTENANCE, h.status().state);
  expectAlarm(h.status(), "A15");
  TEST_ASSERT_TRUE(store.snapshot.a15Active);
  expectJournalCount(store, PersistentEventCode::A15, 1);
}

void test_persist_005_critical_level_and_reset_ok_are_persisted() {
  FakePersistentStore store;
  ControllerHarness h(&store);
  h.tick();

  h.inputs.epLavage = true;
  h.inputs.epCritique = true;
  h.tick();
  h.advance(h.config.epCritiqueDebounceMs);

  TEST_ASSERT_EQUAL(SystemState::FAULT, h.status().state);
  expectAlarm(h.status(), "A01");
  expectJournalCount(store, PersistentEventCode::A01, 1);

  h.inputs.epCritique = false;
  h.tick();
  h.advance(h.config.epCritiqueDebounceMs);
  h.pressReset();

  TEST_ASSERT_EQUAL(SystemState::AUTO_WAIT, h.status().state);
  expectNoAlarm(h.status());
  expectJournalCount(store, PersistentEventCode::RESET_OK, 1);
  TEST_ASSERT_EQUAL(PersistentEventCode::RESET_OK, store.snapshot.lastEvent);
}

void test_persist_006_incoherent_level_is_persisted_once_until_reset() {
  FakePersistentStore store;
  ControllerHarness h(&store);
  h.tick();

  h.inputs.epCritique = true;
  h.tick();
  h.advance(h.config.epCritiqueDebounceMs);
  h.tick();
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::FAULT, h.status().state);
  expectAlarm(h.status(), "A02");
  expectJournalCount(store, PersistentEventCode::A02, 1);
}

void test_persist_007_a03_is_persisted_once_per_capot_danger_occurrence() {
  FakePersistentStore store;
  ControllerHarness h(&store);
  h.tick();
  h.startAutoWash();

  h.inputs.capotOuvert = true;
  h.tick();
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::FAULT, h.status().state);
  expectAlarm(h.status(), "A03");
  expectJournalCount(store, PersistentEventCode::A03, 1);
}

void test_persist_008_a04_is_persisted_once_per_wash_failure_occurrence() {
  FakePersistentStore store;
  ControllerHarness h(&store);
  h.tick();
  h.startAutoWash();

  h.advance(h.config.washMaxMs);
  h.advance(h.config.retryPauseMs);
  h.advance(h.config.washMaxMs);
  h.advance(h.config.retryPauseMs);
  h.advance(h.config.washMaxMs);
  h.tick();
  h.tick();

  TEST_ASSERT_EQUAL(SystemState::DEGRADED, h.status().state);
  expectAlarm(h.status(), "A04");
  expectJournalCount(store, PersistentEventCode::A04, 1);
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  UNITY_BEGIN();
  RUN_TEST(test_sim_001_boot_nominal);
  RUN_TEST(test_sim_002_capot_open_switches_to_maintenance);
  RUN_TEST(test_sim_003_critical_level_incoherence_latches_until_reset);
  RUN_TEST(test_sim_004_005_auto_wash_nominal_then_returns_to_auto_wait);
  RUN_TEST(test_sim_006_persistent_wash_demand_retries_then_degrades_with_a04);
  RUN_TEST(test_sim_007_capot_open_during_wash_triggers_a03_and_blocks_reset);
  RUN_TEST(test_sim_008_capot_fault_can_reset_after_capot_close_stable);
  RUN_TEST(test_sim_009_test_refused_when_capot_open);
  RUN_TEST(test_sim_010_test_refused_when_level_safety_fault_active);
  RUN_TEST(test_sim_011_test_wash_ok_returns_success_message);
  RUN_TEST(test_sim_012_manual_tambour_command_runs_in_manual_mode);
  RUN_TEST(test_sim_013_capot_open_during_manual_command_triggers_a03);
  RUN_TEST(test_sim_014_missing_water_temperature_warns_without_stopping_nominal_outputs);
  RUN_TEST(test_sim_015_missing_local_temperature_warns_without_stopping_nominal_outputs);
  RUN_TEST(test_sim_016_failed_test_wash_does_not_latch_a04);
  RUN_TEST(test_persist_001_boot_is_counted_on_each_begin);
  RUN_TEST(test_persist_002_a15_active_reappears_after_reboot_without_double_counting);
  RUN_TEST(test_persist_003_a15_active_is_cleared_after_reboot_with_closed_capot);
  RUN_TEST(test_persist_004_open_capot_without_persisted_a15_restarts_long_open_timer);
  RUN_TEST(test_persist_005_critical_level_and_reset_ok_are_persisted);
  RUN_TEST(test_persist_006_incoherent_level_is_persisted_once_until_reset);
  RUN_TEST(test_persist_007_a03_is_persisted_once_per_capot_danger_occurrence);
  RUN_TEST(test_persist_008_a04_is_persisted_once_per_wash_failure_occurrence);
  return UNITY_END();
}
