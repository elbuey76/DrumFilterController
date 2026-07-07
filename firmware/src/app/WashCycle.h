#pragma once

#include "Config.h"
#include "Types.h"

struct WashCycleResult {
  SystemState state = SystemState::AUTO_WAIT;
  const char* message = "AUTO - ATTENTE";
  const char* alarmCode = nullptr;

  bool cmdTambour = false;
  bool cmdRincage = false;
  bool voyantLavage = false;
  bool voyantAlarme = false;
};

class WashCycle {
public:
  explicit WashCycle(const Config& config);

  WashCycleResult update(const InputsSnapshot& inputs, unsigned long nowMs);
  void abort();
  void resetAlarm();
  bool hasBlockingAlarm() const;

private:
  enum class Phase {
    Idle,
    Washing,
    PostWash,
    SafetyPause,
    RetryPause,
    Degraded
  };

  void startWash(unsigned long nowMs);
  bool lavageConfirmed(const InputsSnapshot& inputs, unsigned long nowMs);

  Config config_;
  Phase phase_ = Phase::Idle;
  unsigned long phaseStartedAtMs_ = 0;
  unsigned long epLavageActiveSinceMs_ = 0;
  bool epLavageWasActive_ = false;
  uint8_t attemptCount_ = 0;
};
