#pragma once

#include "Config.h"
#include "Types.h"

struct SafetyStatus {
  bool capotOpen = false;
  bool capotOpenLong = false;
  bool levelCritical = false;
  bool levelIncoherent = false;
};

class Safety {
public:
  explicit Safety(const Config& config);

  SafetyStatus update(const InputsSnapshot& inputs, unsigned long nowMs);

private:
  bool updateDebounced(bool rawValue, bool& stableValue, bool& candidateValue, unsigned long& candidateSinceMs, unsigned long delayMs, unsigned long nowMs);

  Config config_;

  bool capotInitialized_ = false;
  bool capotStableOpen_ = false;
  bool capotCandidateOpen_ = false;
  unsigned long capotCandidateSinceMs_ = 0;
  unsigned long capotStableOpenSinceMs_ = 0;

  bool criticalInitialized_ = false;
  bool criticalStable_ = false;
  bool criticalCandidate_ = false;
  unsigned long criticalCandidateSinceMs_ = 0;
};
