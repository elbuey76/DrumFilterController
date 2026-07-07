#include "Safety.h"

Safety::Safety(const Config& config) : config_(config) {}

SafetyStatus Safety::update(const InputsSnapshot& inputs, unsigned long nowMs) {
  if (!capotInitialized_) {
    capotInitialized_ = true;
    capotStableOpen_ = inputs.capotOuvert;
    capotCandidateOpen_ = inputs.capotOuvert;
    capotCandidateSinceMs_ = nowMs;
    capotStableOpenSinceMs_ = inputs.capotOuvert ? nowMs : 0;
  }

  if (!criticalInitialized_) {
    criticalInitialized_ = true;
    criticalStable_ = inputs.epCritique;
    criticalCandidate_ = inputs.epCritique;
    criticalCandidateSinceMs_ = nowMs;
  }

  const unsigned long capotDelayMs = inputs.capotOuvert ? config_.capotOpenDebounceMs : config_.capotCloseStableMs;
  const bool capotChanged = updateDebounced(inputs.capotOuvert, capotStableOpen_, capotCandidateOpen_, capotCandidateSinceMs_, capotDelayMs, nowMs);
  if (capotChanged && capotStableOpen_) {
    capotStableOpenSinceMs_ = nowMs;
  }

  updateDebounced(inputs.epCritique, criticalStable_, criticalCandidate_, criticalCandidateSinceMs_, config_.epCritiqueDebounceMs, nowMs);

  SafetyStatus status;
  status.capotOpen = capotStableOpen_;
  status.capotOpenLong = capotStableOpen_ && (nowMs - capotStableOpenSinceMs_ >= config_.capotLongOpenMs);
  status.levelCritical = criticalStable_;
  status.levelIncoherent = criticalStable_ && !inputs.epLavage;
  return status;
}

bool Safety::updateDebounced(bool rawValue, bool& stableValue, bool& candidateValue, unsigned long& candidateSinceMs, unsigned long delayMs, unsigned long nowMs) {
  if (rawValue != candidateValue) {
    candidateValue = rawValue;
    candidateSinceMs = nowMs;
  }

  if (stableValue != candidateValue && nowMs - candidateSinceMs >= delayMs) {
    stableValue = candidateValue;
    return true;
  }

  return false;
}
