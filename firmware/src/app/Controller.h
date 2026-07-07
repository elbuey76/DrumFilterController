#pragma once

#include "Config.h"
#include "Safety.h"
#include "Types.h"
#include "WashCycle.h"

class Controller {
public:
  explicit Controller(const Config& config);

  void begin(unsigned long nowMs);
  OutputsCommand update(const InputsSnapshot& inputs, unsigned long nowMs);
  ControllerStatus status() const;

private:
  OutputsCommand safeOutputs() const;
  OutputsCommand nominalOutputs() const;
  OutputsCommand maintenanceOutputs() const;
  OutputsCommand manualOutputs(const InputsSnapshot& inputs) const;
  bool resetAllowed(const InputsSnapshot& inputs, const SafetyStatus& safety) const;
  const char* resetRefusalMessage(const InputsSnapshot& inputs, const SafetyStatus& safety) const;
  void setStatus(SystemState state, const char* message, const char* alarmCode = nullptr);
  OutputsCommand finishUpdate(const OutputsCommand& outputs);

  Safety safety_;
  WashCycle washCycle_;
  bool capotDangerFaultLatched_ = false;
  bool dangerousOutputWasActive_ = false;
  SystemState state_ = SystemState::BOOT;
  ControllerStatus status_{};
};
