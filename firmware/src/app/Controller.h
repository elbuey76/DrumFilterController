#pragma once

#include "Config.h"
#include "StateMachine.h"
#include "Types.h"

class Controller {
public:
  explicit Controller(const Config& config);

  void begin(unsigned long nowMs);
  OutputsCommand update(const InputsSnapshot& inputs, unsigned long nowMs);
  ControllerStatus status() const;

private:
  OutputsCommand safeOutputs() const;
  OutputsCommand nominalOutputs() const;
  void setStatus(SystemState state, const char* message, const char* alarmCode = nullptr);

  Config config_;
  StateMachine stateMachine_;
  SystemState state_ = SystemState::BOOT;
  ControllerStatus status_{};
};
