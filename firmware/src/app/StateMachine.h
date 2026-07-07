#pragma once

#include "Types.h"

struct StateDecision {
  SystemState state = SystemState::BOOT;
  const char* message = "BOOT";
  const char* alarmCode = nullptr;
};

class StateMachine {
public:
  StateDecision decide(const InputsSnapshot& inputs) const;
};
