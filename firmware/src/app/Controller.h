#pragma once

#include "AlarmManager.h"
#include "Config.h"
#include "PersistentStore.h"
#include "Safety.h"
#include "Types.h"
#include "WashCycle.h"

class Controller {
public:
  explicit Controller(const Config& config, PersistentStore* persistentStore = nullptr);

  void begin(unsigned long nowMs);
  OutputsCommand update(const InputsSnapshot& inputs, unsigned long nowMs);
  ControllerStatus status() const;
  PersistentJournalSnapshot journalSnapshot() const;

private:
  OutputsCommand safeOutputs() const;
  OutputsCommand nominalOutputs() const;
  OutputsCommand maintenanceOutputs() const;
  OutputsCommand manualOutputs(const InputsSnapshot& inputs) const;
  OutputsCommand washOutputs(const WashCycleResult& wash) const;
  bool resetAllowed(const InputsSnapshot& inputs, const SafetyStatus& safety) const;
  const char* resetRefusalMessage(const InputsSnapshot& inputs, const SafetyStatus& safety) const;
  AlarmCode temperatureAlarm(const InputsSnapshot& inputs) const;
  OutputsCommand testRefusedOutputs(bool blocking) const;
  bool capotOpen(const InputsSnapshot& inputs, const SafetyStatus& safety) const;
  void recordPersistentEvent(PersistentEventCode eventCode);
  void setPersistentA15Active(bool active);
  void latchBlockingAlarm(AlarmCode alarmCode);
  PersistentEventCode persistentEventForAlarm(AlarmCode alarmCode) const;
  void setStatus(SystemState state, const char* message, const char* alarmCode = nullptr);
  void setAlarmStatus(SystemState state, AlarmCode alarmCode, const char* messageOverride = nullptr);
  OutputsCommand finishUpdate(const OutputsCommand& outputs);

  AlarmManager alarmManager_;
  Safety safety_;
  WashCycle washCycle_;
  NullPersistentStore nullPersistentStore_;
  PersistentStore* persistentStore_;
  PersistentJournalSnapshot journal_{};
  AlarmCode blockingAlarmLatched_ = AlarmCode::NONE;
  bool washAlarmPersisted_ = false;
  bool dangerousOutputWasActive_ = false;
  SystemState state_ = SystemState::BOOT;
  ControllerStatus status_{};
};
