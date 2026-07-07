#include "Controller.h"

Controller::Controller(const Config& config) : safety_(config), washCycle_(config) {}

void Controller::begin(unsigned long /*nowMs*/) {
  setStatus(SystemState::BOOT, "BOOT");
}

OutputsCommand Controller::update(const InputsSnapshot& inputs, unsigned long nowMs) {
  const SafetyStatus safety = safety_.update(inputs, nowMs);

  if (capotOpen(inputs, safety) && dangerousOutputWasActive_) {
    blockingAlarmLatched_ = AlarmCode::A03;
  }

  if (inputs.btnReset) {
    if (!resetAllowed(inputs, safety)) {
      setAlarmStatus(SystemState::FAULT, AlarmCode::A05, resetRefusalMessage(inputs, safety));
      return finishUpdate(safeOutputs());
    }

    blockingAlarmLatched_ = AlarmCode::NONE;
    washCycle_.resetAlarm();
  }

  if (inputs.btnTestLavage) {
    if (capotOpen(inputs, safety)) {
      setAlarmStatus(SystemState::MAINTENANCE, AlarmCode::A13);
      return finishUpdate(testRefusedOutputs(false));
    }

    if (safety.levelCritical || safety.levelIncoherent || blockingAlarmLatched_ != AlarmCode::NONE || (!inputs.modeAuto && !inputs.modeMaintenance)) {
      setAlarmStatus(SystemState::FAULT, AlarmCode::A14);
      return finishUpdate(testRefusedOutputs(true));
    }

    if (!washCycle_.startTest(inputs.epLavage, nowMs)) {
      setAlarmStatus(SystemState::FAULT, AlarmCode::A14, "A14 - TEST REFUSE CYCLE ACTIF");
      return finishUpdate(testRefusedOutputs(true));
    }
  }

  if (safety.levelIncoherent) {
    blockingAlarmLatched_ = AlarmCode::A02;
  }

  if (!safety.levelIncoherent && safety.levelCritical) {
    blockingAlarmLatched_ = AlarmCode::A01;
  }

  if (blockingAlarmLatched_ != AlarmCode::NONE) {
    washCycle_.abort();
    setAlarmStatus(SystemState::FAULT, blockingAlarmLatched_);
    return finishUpdate(safeOutputs());
  }

  if (washCycle_.hasBlockingAlarm()) {
    WashCycleResult degraded;
    degraded.state = SystemState::DEGRADED;
    degraded.message = alarmManager_.message(AlarmCode::A04);
    degraded.alarmCode = AlarmCode::A04;
    degraded.voyantAlarme = true;
    setAlarmStatus(SystemState::DEGRADED, AlarmCode::A04);
    return finishUpdate(washOutputs(degraded));
  }

  if (capotOpen(inputs, safety)) {
    washCycle_.abort();
    OutputsCommand outputs = maintenanceOutputs();
    if (safety.capotOpenLong) {
      outputs.voyantAlarme = true;
      setAlarmStatus(SystemState::MAINTENANCE, AlarmCode::A15);
    } else if (inputs.btnManuTambour || inputs.btnManuRincage) {
      setStatus(SystemState::MAINTENANCE, "MANU REFUSE - CAPOT");
    } else {
      setStatus(SystemState::MAINTENANCE, "MAINTENANCE - CAPOT OUVERT");
    }
    return finishUpdate(outputs);
  }

  if ((inputs.modeMaintenance || !inputs.modeAuto) && !washCycle_.isTestRunning()) {
    washCycle_.abort();

    if (inputs.btnManuTambour || inputs.btnManuRincage) {
      setStatus(SystemState::MANUAL, "COMMANDE MANUELLE");
      return finishUpdate(manualOutputs(inputs));
    }

    setStatus(inputs.modeMaintenance ? SystemState::MAINTENANCE : SystemState::MANUAL, inputs.modeMaintenance ? "MAINTENANCE" : "MANUEL");
    return finishUpdate(maintenanceOutputs());
  }

  WashCycleResult wash = washCycle_.update(inputs, nowMs);
  OutputsCommand outputs = washOutputs(wash);
  if (wash.alarmCode != AlarmCode::NONE) {
    setAlarmStatus(wash.state, wash.alarmCode, wash.message);
    return finishUpdate(outputs);
  }

  const AlarmCode tempAlarm = temperatureAlarm(inputs);
  if (tempAlarm != AlarmCode::NONE) {
    outputs.voyantAlarme = true;
    setAlarmStatus(wash.state, tempAlarm);
    return finishUpdate(outputs);
  }

  setStatus(wash.state, wash.message);
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

OutputsCommand Controller::washOutputs(const WashCycleResult& wash) const {
  OutputsCommand outputs = nominalOutputs();
  outputs.cmdTambour = wash.cmdTambour;
  outputs.cmdRincage = wash.cmdRincage;
  outputs.voyantLavage = wash.voyantLavage;
  outputs.voyantAlarme = wash.voyantAlarme;
  return outputs;
}

bool Controller::resetAllowed(const InputsSnapshot& inputs, const SafetyStatus& safety) const {
  if (safety.levelCritical || safety.levelIncoherent || (blockingAlarmLatched_ == AlarmCode::A03 && capotOpen(inputs, safety))) {
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

  if (blockingAlarmLatched_ == AlarmCode::A03 && capotOpen(inputs, safety)) {
    return "A05 - RESET REFUSE - CAPOT OUVERT";
  }

  if (washCycle_.hasBlockingAlarm() && inputs.epLavage) {
    return "A05 - RESET REFUSE - EP_LAVAGE ACTIF";
  }

  return "A05 - RESET REFUSE";
}

AlarmCode Controller::temperatureAlarm(const InputsSnapshot& inputs) const {
  if (!inputs.tempBassinValid) {
    return AlarmCode::A11;
  }

  if (!inputs.tempLocalValid) {
    return AlarmCode::A12;
  }

  return AlarmCode::NONE;
}

OutputsCommand Controller::testRefusedOutputs(bool blocking) const {
  OutputsCommand outputs = blocking ? safeOutputs() : maintenanceOutputs();
  outputs.voyantAlarme = true;
  return outputs;
}

bool Controller::capotOpen(const InputsSnapshot& inputs, const SafetyStatus& safety) const {
  return inputs.capotOuvert || safety.capotOpen;
}

void Controller::setStatus(SystemState state, const char* message, const char* alarmCode) {
  state_ = state;
  status_.state = state;
  status_.message = message;
  status_.alarmCode = alarmCode;
}

void Controller::setAlarmStatus(SystemState state, AlarmCode alarmCode, const char* messageOverride) {
  setStatus(state, messageOverride == nullptr ? alarmManager_.message(alarmCode) : messageOverride, alarmManager_.codeText(alarmCode));
}

OutputsCommand Controller::finishUpdate(const OutputsCommand& outputs) {
  dangerousOutputWasActive_ = outputs.cmdTambour || outputs.cmdRincage;
  return outputs;
}
