#include "WashCycle.h"

WashCycle::WashCycle(const Config& config) : config_(config) {}

WashCycleResult WashCycle::update(const InputsSnapshot& inputs, unsigned long nowMs) {
  WashCycleResult result;

  if (phase_ == Phase::Idle) {
    if (!lavageConfirmed(inputs, nowMs)) {
      result.message = inputs.epLavage ? "AUTO - DEMANDE LAVAGE EN TEMPORISATION" : "AUTO - ATTENTE";
      return result;
    }

    startWash(nowMs);
  }

  for (;;) {
    switch (phase_) {
      case Phase::Washing:
        if (nowMs - phaseStartedAtMs_ >= config_.washMaxMs) {
          if (attemptCount_ >= config_.maxWashAttempts) {
            phase_ = Phase::Degraded;
            phaseStartedAtMs_ = nowMs;
          } else {
            phase_ = Phase::RetryPause;
            phaseStartedAtMs_ = nowMs;
          }
          continue;
        }

        if (nowMs - phaseStartedAtMs_ >= config_.washMinMs && !inputs.epLavage) {
          phase_ = Phase::PostWash;
          phaseStartedAtMs_ = nowMs;
          continue;
        }

        result.state = SystemState::WASH_AUTO;
        result.message = "LAVAGE AUTO";
        result.cmdTambour = true;
        result.cmdRincage = true;
        result.voyantLavage = true;
        return result;

      case Phase::PostWash:
        if (nowMs - phaseStartedAtMs_ >= config_.residualRotationMs) {
          phase_ = Phase::SafetyPause;
          phaseStartedAtMs_ = nowMs;
          continue;
        }

        result.state = SystemState::POST_WASH;
        result.message = "POST-LAVAGE - ROTATION RESIDUELLE";
        result.cmdTambour = true;
        result.voyantLavage = true;
        return result;

      case Phase::SafetyPause:
        if (nowMs - phaseStartedAtMs_ >= config_.antiRestartMs) {
          phase_ = Phase::Idle;
          attemptCount_ = 0;
          epLavageWasActive_ = false;
          continue;
        }

        result.state = SystemState::SAFETY_PAUSE;
        result.message = "PAUSE ANTI-REDEMARRAGE";
        return result;

      case Phase::RetryPause:
        if (nowMs - phaseStartedAtMs_ >= config_.retryPauseMs) {
          if (inputs.epLavage) {
            startWash(nowMs);
          } else {
            phase_ = Phase::Idle;
            attemptCount_ = 0;
            epLavageWasActive_ = false;
          }
          continue;
        }

        result.state = SystemState::RETRY_PAUSE;
        result.message = "LAVAGE INEFFICACE - PAUSE AVANT RETENTATIVE";
        return result;

      case Phase::Degraded:
        result.state = SystemState::DEGRADED;
        result.message = "A04 - LAVAGE INEFFICACE";
        result.alarmCode = "A04";
        result.voyantAlarme = true;
        return result;

      case Phase::Idle:
        result.message = inputs.epLavage ? "AUTO - DEMANDE LAVAGE EN TEMPORISATION" : "AUTO - ATTENTE";
        return result;
    }
  }
}

void WashCycle::abort() {
  if (phase_ != Phase::Degraded) {
    phase_ = Phase::Idle;
    attemptCount_ = 0;
    epLavageWasActive_ = false;
  }
}

void WashCycle::resetAlarm() {
  phase_ = Phase::Idle;
  attemptCount_ = 0;
  epLavageWasActive_ = false;
}

bool WashCycle::hasBlockingAlarm() const {
  return phase_ == Phase::Degraded;
}

void WashCycle::startWash(unsigned long nowMs) {
  phase_ = Phase::Washing;
  phaseStartedAtMs_ = nowMs;
  ++attemptCount_;
  epLavageWasActive_ = false;
}

bool WashCycle::lavageConfirmed(const InputsSnapshot& inputs, unsigned long nowMs) {
  if (!inputs.epLavage) {
    epLavageWasActive_ = false;
    return false;
  }

  if (!epLavageWasActive_) {
    epLavageWasActive_ = true;
    epLavageActiveSinceMs_ = nowMs;
  }

  return nowMs - epLavageActiveSinceMs_ >= config_.epLavageDelayMs;
}
