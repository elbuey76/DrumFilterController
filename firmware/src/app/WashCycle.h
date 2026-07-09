#pragma once

#include "Config.h"
#include "Types.h"

struct WashCycleResult {
  SystemState state = SystemState::AUTO_WAIT;
  const char* message = "AUTO - ATTENTE";
  AlarmCode alarmCode = AlarmCode::NONE;

  bool cmdTambour = false;
  bool cmdRincage = false;
  bool voyantLavage = false;
  bool voyantAlarme = false;
};

class WashCycle {
public:
  explicit WashCycle(const Config& config);

  WashCycleResult update(const InputsSnapshot& inputs, unsigned long nowMs);
  bool startTest(bool epLavageActiveAtStart, unsigned long nowMs);
  void abort();
  void resetAlarm();
  bool hasBlockingAlarm() const;
  bool isTestRunning() const;
  bool isTestActive() const;

private:
  enum class Phase {
    Idle,
    Washing,
    PostWash,
    SafetyPause,
    RetryPause,
    TestWashing,
    TestDone,
    Degraded
  };

  void startWash(unsigned long nowMs);
  bool lavageConfirmed(const InputsSnapshot& inputs, unsigned long nowMs);

  Config config_;
  Phase phase_ = Phase::Idle;
  unsigned long phaseStartedAtMs_ = 0;
  unsigned long epLavageActiveSinceMs_ = 0;
  const char* testResultMessage_ = nullptr;
  bool epLavageWasActive_ = false;
  bool testStartedWithEpLavage_ = false;
  uint8_t attemptCount_ = 0;
};
