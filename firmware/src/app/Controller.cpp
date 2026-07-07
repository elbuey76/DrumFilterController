#include "Controller.h"

Controller::Controller(const Config& config) : config_(config) {}

void Controller::begin(unsigned long /*nowMs*/) {
  setStatus(SystemState::BOOT, "BOOT");
}

OutputsCommand Controller::update(const InputsSnapshot& inputs, unsigned long /*nowMs*/) {
  const StateDecision decision = stateMachine_.decide(inputs);
  setStatus(decision.state, decision.message, decision.alarmCode);

  if (decision.state == SystemState::FAULT) {
    return safeOutputs();
  }

  OutputsCommand outputs = nominalOutputs();

  if (decision.state == SystemState::MAINTENANCE) {
    outputs.cmdTambour = false;
    outputs.cmdRincage = false;
    outputs.voyantLavage = false;
    return outputs;
  }

  if (decision.state == SystemState::DEGRADED) {
    outputs.cmdTambour = false;
    outputs.cmdRincage = false;
    outputs.voyantLavage = false;
    outputs.voyantAlarme = true;
    return outputs;
  }

  if (decision.state == SystemState::MANUAL) {
    outputs.cmdTambour = inputs.btnManuTambour;
    outputs.cmdRincage = inputs.btnManuRincage;
    outputs.voyantLavage = outputs.cmdTambour || outputs.cmdRincage;
  }

  return outputs;
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

void Controller::setStatus(SystemState state, const char* message, const char* alarmCode) {
  state_ = state;
  status_.state = state;
  status_.message = message;
  status_.alarmCode = alarmCode;
}
