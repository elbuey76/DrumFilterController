#include "StateMachine.h"

StateDecision StateMachine::decide(const InputsSnapshot& inputs) const {
  if (inputs.epCritique) {
    return {SystemState::FAULT, "A01 - NIVEAU CRITIQUE", "A01"};
  }

  if (inputs.capotOuvert) {
    return {SystemState::MAINTENANCE, "MAINTENANCE - CAPOT OUVERT", nullptr};
  }

  if (inputs.modeMaintenance) {
    return {SystemState::MAINTENANCE, "MAINTENANCE", nullptr};
  }

  if (inputs.modeAuto) {
    if (inputs.epLavage) {
      return {SystemState::DEGRADED, "EP_LAVAGE ACTIF - LAVAGE A IMPLEMENTER", "A10"};
    }

    return {SystemState::AUTO_WAIT, "AUTO - ATTENTE", nullptr};
  }

  return {SystemState::MANUAL, "MANUEL", nullptr};
}
