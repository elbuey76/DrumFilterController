#include "Controller.h"

Controller::Controller(const Config& config) : safety_(config), washCycle_(config) {}

void Controller::begin(unsigned long /*nowMs*/) {
  setStatus(SystemState::BOOT, "BOOT");
}

OutputsCommand Controller::update(const InputsSnapshot& inputs, unsigned long nowMs) {
  const SafetyStatus safety = safety_.update(inputs, nowMs);

  if (safety.capotOpen && dangerousOutputWasActive_) {
    capotDangerFaultLatched_ = true;
  }

  if (inputs.btnReset) {
    if (!resetAllowed(inputs, safety)) {
      setStatus(SystemState::FAULT, resetRefusalMessage(inputs, safety), "A05");
      return finishUpdate(safeOutputs());
    }

    capotDangerFaultLatched_ = false;
    washCycle_.resetAlarm();
  }

  if (safety.levelIncoherent) {
    washCycle_.abort();
    setStatus(SystemState::FAULT, "A02 - CAPTEURS NIVEAU INCOHERENTS", "A02");
    return finishUpdate(safeOutputs());
  }

  if (safety.levelCritical) {
    washCycle_.abort();
    setStatus(SystemState::FAULT, "A01 - NIVEAU CRITIQUE", "A01");
    return finishUpdate(safeOutputs());
  }

  if (capotDangerFaultLatched_) {
    washCycle_.abort();
    setStatus(SystemState::FAULT, "A03 - CAPOT OUVERT DANGER", "A03");
    return finishUpdate(safeOutputs());
  }

  if (safety.capotOpen) {
    washCycle_.abort();
    OutputsCommand outputs = maintenanceOutputs();
    if (safety.capotOpenLong) {
      outputs.voyantAlarme = true;
      setStatus(SystemState::MAINTENANCE, "A15 - CAPOT OUVERT LONG", "A15");
    } else if (inputs.btnManuTambour || inputs.btnManuRincage) {
      setStatus(SystemState::MAINTENANCE, "MANU REFUSE - CAPOT");
    } else {
      setStatus(SystemState::MAINTENANCE, "MAINTENANCE - CAPOT OUVERT");
    }
    return finishUpdate(outputs);
  }

  if (inputs.modeMaintenance || !inputs.modeAuto) {
    washCycle_.abort();

    if (inputs.btnManuTambour || inputs.btnManuRincage) {
      setStatus(SystemState::MANUAL, "COMMANDE MANUELLE");
      return finishUpdate(manualOutputs(inputs));
    }

    setStatus(inputs.modeMaintenance ? SystemState::MAINTENANCE : SystemState::MANUAL, inputs.modeMaintenance ? "MAINTENANCE" : "MANUEL");
    return finishUpdate(maintenanceOutputs());
  }

  WashCycleResult wash = washCycle_.update(inputs, nowMs);
  OutputsCommand outputs = nominalOutputs();
  outputs.cmdTambour = wash.cmdTambour;
  outputs.cmdRincage = wash.cmdRincage;
  outputs.voyantLavage = wash.voyantLavage;
  outputs.voyantAlarme = wash.voyantAlarme;

  setStatus(wash.state, wash.message, wash.alarmCode);
  return finishUpdate(outputs);
}

ControllerStatus Controller::status() const {
  return status_;
}

OutputsCommand Controller::safeOutputs() const {
  OutputsCommand outputs{};
  outputs.voyantAlarme = true;
  return outputs;
}

OutputsCommand Controller::nominalOutputs() const {
  OutputsCommand outputs{};
  outputs.cmdPompeFiltration = true;
  outputs.cmdPompeDeco = true;
  outputs.cmdUv = true;
  outputs.cmdMiseANiveau = true;
  outputs.voyantMarche = true;
  return outputs;
}

OutputsCommand Controller::maintenanceOutputs() const {
  OutputsCommand outputs = nominalOutputs();
  outputs.cmdTambour = false;
  outputs.cmdRincage = false;
  outputs.voyantLavage = false;
  return outputs;
}

OutputsCommand Controller::manualOutputs(const InputsSnapshot& inputs) const {
  OutputsCommand outputs = nominalOutputs();
  outputs.cmdTambour = inputs.btnManuTambour;
  outputs.cmdRincage = inputs.btnManuRincage;
  outputs.voyantLavage = outputs.cmdTambour || outputs.cmdRincage;
  return outputs;
}

bool Controller::resetAllowed(const InputsSnapshot& inputs, const SafetyStatus& safety) const {
  if (safety.levelCritical || safety.levelIncoherent || (capotDangerFaultLatched_ && safety.capotOpen)) {
    return false;
  }

  if (washCycle_.hasBlockingAlarm() && inputs.epLavage) {
    return false;
  }

  return true;
}

const char* Controller::resetRefusalMessage(const InputsSnapshot& inputs, const SafetyStatus& safety) const {
  if (safety.levelIncoherent) {
    return "A05 - RESET REFUSE - CAPTEURS NIVEAU";
  }

  if (safety.levelCritical) {
    return "A05 - RESET REFUSE - EP_CRITIQUE ACTIF";
  }

  if (capotDangerFaultLatched_ && safety.capotOpen) {
    return "A05 - RESET REFUSE - CAPOT OUVERT";
  }

  if (washCycle_.hasBlockingAlarm() && inputs.epLavage) {
    return "A05 - RESET REFUSE - EP_LAVAGE ACTIF";
  }

  return "A05 - RESET REFUSE";
}

void Controller::setStatus(SystemState state, const char* message, const char* alarmCode) {
  state_ = state;
  status_.state = state;
  status_.message = message;
  status_.alarmCode = alarmCode;
}

OutputsCommand Controller::finishUpdate(const OutputsCommand& outputs) {
  dangerousOutputWasActive_ = outputs.cmdTambour || outputs.cmdRincage;
  return outputs;
}
