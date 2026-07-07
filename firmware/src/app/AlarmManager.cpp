#include "AlarmManager.h"

namespace {
const AlarmDefinition kNoAlarm{AlarmCode::NONE, nullptr, "", AlarmSeverity::INFO};
const AlarmDefinition kA01{AlarmCode::A01, "A01", "A01 - NIVEAU CRITIQUE", AlarmSeverity::BLOCKING};
const AlarmDefinition kA02{AlarmCode::A02, "A02", "A02 - CAPTEURS NIVEAU INCOHERENTS", AlarmSeverity::BLOCKING};
const AlarmDefinition kA03{AlarmCode::A03, "A03", "A03 - CAPOT OUVERT DANGER", AlarmSeverity::BLOCKING};
const AlarmDefinition kA04{AlarmCode::A04, "A04", "A04 - LAVAGE INEFFICACE", AlarmSeverity::DEGRADED};
const AlarmDefinition kA05{AlarmCode::A05, "A05", "A05 - RESET REFUSE", AlarmSeverity::WARNING};
const AlarmDefinition kA11{AlarmCode::A11, "A11", "A11 - SONDE EAU ABSENTE", AlarmSeverity::WARNING};
const AlarmDefinition kA12{AlarmCode::A12, "A12", "A12 - SONDE LOCAL ABSENTE", AlarmSeverity::WARNING};
const AlarmDefinition kA13{AlarmCode::A13, "A13", "A13 - TEST REFUSE CAPOT", AlarmSeverity::WARNING};
const AlarmDefinition kA14{AlarmCode::A14, "A14", "A14 - TEST REFUSE SECURITE", AlarmSeverity::WARNING};
const AlarmDefinition kA15{AlarmCode::A15, "A15", "A15 - CAPOT OUVERT LONG", AlarmSeverity::WARNING};
}  // namespace

const AlarmDefinition& AlarmManager::definition(AlarmCode code) const {
  switch (code) {
    case AlarmCode::A01: return kA01;
    case AlarmCode::A02: return kA02;
    case AlarmCode::A03: return kA03;
    case AlarmCode::A04: return kA04;
    case AlarmCode::A05: return kA05;
    case AlarmCode::A11: return kA11;
    case AlarmCode::A12: return kA12;
    case AlarmCode::A13: return kA13;
    case AlarmCode::A14: return kA14;
    case AlarmCode::A15: return kA15;
    case AlarmCode::NONE: return kNoAlarm;
  }

  return kNoAlarm;
}

const char* AlarmManager::codeText(AlarmCode code) const {
  return definition(code).codeText;
}

const char* AlarmManager::message(AlarmCode code) const {
  return definition(code).message;
}

AlarmSeverity AlarmManager::severity(AlarmCode code) const {
  return definition(code).severity;
}
