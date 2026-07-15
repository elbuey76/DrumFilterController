#include "DisplayService.h"

#include <stdio.h>
#include <string.h>

#if USE_A16_AUXILIARIES
#include <LCDi2c.h>
#endif

void DisplayService::begin(Stream& stream, LCDi2c* lcd) {
  stream_ = &stream;
  firstRender_ = true;
#if USE_A16_AUXILIARIES
  lcd_ = lcd;
  if (lcd_ != nullptr) {
    lcd_->begin(4, 20);
    lcd_->display(BACKLIGHT_ON);
  }
#else
  (void)lcd;
#endif
}

void DisplayService::render(const ControllerStatus& status, const InputsSnapshot& inputs, const OutputsCommand& outputs) {
  if (stream_ == nullptr || !hasChanged(status, inputs, outputs)) {
    return;
  }

  stream_->println(F("--- DISPLAY ---"));
  stream_->print(F("MODE: "));
  stream_->println(modeName(inputs, status.state));
  stream_->print(F("NIVEAU: "));
  stream_->println(levelName(inputs));
  stream_->print(F("LAVAGE: "));
  stream_->println(outputs.cmdTambour || outputs.cmdRincage ? F("ACTIF") : F("REPOS"));
  stream_->print(F("ALARME: "));
  if (status.alarmCode == nullptr) {
    stream_->println(F("-"));
  } else {
    stream_->println(status.alarmCode);
  }
  stream_->print(F("ETAT: "));
  stream_->println(stateName(status.state));
  stream_->print(F("MSG: "));
  stream_->println(status.message);
  stream_->print(F("TEMP EAU: "));
  if (inputs.tempBassinValid) {
    stream_->print(inputs.tempBassinC, 1);
    stream_->println(F(" C"));
  } else {
    stream_->println(F("ABSENTE"));
  }
  stream_->print(F("TEMP LOCAL: "));
  if (inputs.tempLocalValid) {
    stream_->print(inputs.tempLocalC, 1);
    stream_->println(F(" C"));
  } else {
    stream_->println(F("ABSENTE"));
  }
  stream_->print(F("OUT T/R/F/UV: "));
  stream_->print(boolText(outputs.cmdTambour));
  stream_->print(F("/"));
  stream_->print(boolText(outputs.cmdRincage));
  stream_->print(F("/"));
  stream_->print(boolText(outputs.cmdPompeFiltration));
  stream_->print(F("/"));
  stream_->println(boolText(outputs.cmdUv));

  renderLcd(status, inputs, outputs);

  lastStatus_ = status;
  lastInputs_ = inputs;
  lastOutputs_ = outputs;
  firstRender_ = false;
}

bool DisplayService::lcdAvailable() const {
#if USE_A16_AUXILIARIES
  return lcd_ != nullptr;
#else
  return false;
#endif
}

const char* DisplayService::stateName(SystemState state) const {
  switch (state) {
    case SystemState::BOOT: return "BOOT";
    case SystemState::AUTO_WAIT: return "AUTO_WAIT";
    case SystemState::WASH_AUTO: return "WASH_AUTO";
    case SystemState::POST_WASH: return "POST_WASH";
    case SystemState::SAFETY_PAUSE: return "SAFETY_PAUSE";
    case SystemState::RETRY_PAUSE: return "RETRY_PAUSE";
    case SystemState::MANUAL: return "MANUAL";
    case SystemState::MAINTENANCE: return "MAINTENANCE";
    case SystemState::TEST_WASH: return "TEST_WASH";
    case SystemState::DEGRADED: return "DEGRADED";
    case SystemState::FAULT: return "FAULT";
  }

  return "UNKNOWN";
}

const char* DisplayService::modeName(const InputsSnapshot& inputs, SystemState state) const {
  if (state == SystemState::FAULT) {
    return "DEFAUT";
  }

  if (state == SystemState::DEGRADED) {
    return "DEGRADE";
  }

  if (inputs.modeMaintenance) {
    return "MAINTENANCE";
  }

  return inputs.modeAuto ? "AUTO" : "MANUEL";
}

const char* DisplayService::levelName(const InputsSnapshot& inputs) const {
  if (inputs.epCritique && !inputs.epLavage) {
    return "INCOHERENT";
  }

  if (inputs.epCritique) {
    return "CRITIQUE";
  }

  return inputs.epLavage ? "LAVAGE" : "OK";
}

const char* DisplayService::boolText(bool value) const {
  return value ? "ON" : "OFF";
}

bool DisplayService::hasChanged(const ControllerStatus& status, const InputsSnapshot& inputs, const OutputsCommand& outputs) const {
  if (firstRender_) {
    return true;
  }

  return status.state != lastStatus_.state || status.message != lastStatus_.message || status.alarmCode != lastStatus_.alarmCode ||
         inputs.epLavage != lastInputs_.epLavage || inputs.epCritique != lastInputs_.epCritique || inputs.capotOuvert != lastInputs_.capotOuvert ||
         inputs.modeAuto != lastInputs_.modeAuto || inputs.modeMaintenance != lastInputs_.modeMaintenance ||
         inputs.tempBassinValid != lastInputs_.tempBassinValid || inputs.tempLocalValid != lastInputs_.tempLocalValid ||
         inputs.tempBassinC != lastInputs_.tempBassinC || inputs.tempLocalC != lastInputs_.tempLocalC ||
         outputs.cmdTambour != lastOutputs_.cmdTambour || outputs.cmdRincage != lastOutputs_.cmdRincage ||
         outputs.cmdPompeFiltration != lastOutputs_.cmdPompeFiltration || outputs.cmdPompeDeco != lastOutputs_.cmdPompeDeco ||
         outputs.cmdUv != lastOutputs_.cmdUv || outputs.cmdMiseANiveau != lastOutputs_.cmdMiseANiveau ||
         outputs.voyantAlarme != lastOutputs_.voyantAlarme;
}

void DisplayService::renderLcd(const ControllerStatus& status, const InputsSnapshot& inputs, const OutputsCommand& outputs) {
#if USE_A16_AUXILIARIES
  if (lcd_ == nullptr) {
    return;
  }

  char line[32]{};
  snprintf(line, sizeof(line), "MODE:%s NIV:%s", modeName(inputs, status.state), levelName(inputs));
  writeLcdLine(0, line);

  char water[10]{};
  if (inputs.tempBassinValid) {
    snprintf(water, sizeof(water), "%.1fC", inputs.tempBassinC);
  } else {
    strncpy(water, "ABS", sizeof(water) - 1);
  }
  snprintf(line, sizeof(line), "LAV:%s EAU:%s", outputs.cmdTambour || outputs.cmdRincage ? "ACTIF" : "REPOS", water);
  writeLcdLine(1, line);

  writeLcdLine(2, status.alarmCode == nullptr ? status.message : status.alarmCode);

  char local[10]{};
  if (inputs.tempLocalValid) {
    snprintf(local, sizeof(local), "%.1fC", inputs.tempLocalC);
  } else {
    strncpy(local, "ABS", sizeof(local) - 1);
  }
  snprintf(line, sizeof(line), "LOC:%s F:%s UV:%s", local, boolText(outputs.cmdPompeFiltration), boolText(outputs.cmdUv));
  writeLcdLine(3, line);
#else
  (void)status;
  (void)inputs;
  (void)outputs;
#endif
}

void DisplayService::writeLcdLine(uint8_t row, const char* text) {
#if USE_A16_AUXILIARIES
  if (lcd_ == nullptr || row >= 4) {
    return;
  }
  char padded[21];
  memset(padded, ' ', 20);
  padded[20] = '\0';
  const size_t length = strlen(text);
  memcpy(padded, text, length < 20 ? length : 20);
  lcd_->locate(row + 1, 1);
  lcd_->print(padded);
#else
  (void)row;
  (void)text;
#endif
}
