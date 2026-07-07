#pragma once

#include "Types.h"

struct AlarmDefinition {
  AlarmCode code = AlarmCode::NONE;
  const char* codeText = nullptr;
  const char* message = "";
  AlarmSeverity severity = AlarmSeverity::INFO;

  AlarmDefinition() = default;
  AlarmDefinition(AlarmCode alarmCode, const char* alarmCodeText, const char* alarmMessage, AlarmSeverity alarmSeverity)
      : code(alarmCode), codeText(alarmCodeText), message(alarmMessage), severity(alarmSeverity) {}
};

class AlarmManager {
public:
  const AlarmDefinition& definition(AlarmCode code) const;
  const char* codeText(AlarmCode code) const;
  const char* message(AlarmCode code) const;
  AlarmSeverity severity(AlarmCode code) const;
};
