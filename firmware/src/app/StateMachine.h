#pragma once

#include "Types.h"

struct StateDecision {
  SystemState state = SystemState::BOOT;
  const char* message = "BOOT";
  const char* alarmCode = nullptr;

  StateDecision() = default;
  StateDecision(SystemState nextState, const char* nextMessage, const char* nextAlarmCode = nullptr)
      : state(nextState), message(nextMessage), alarmCode(nextAlarmCode) {}
};

class StateMachine {
public:
  StateDecision decide(const InputsSnapshot& inputs) const;
};
