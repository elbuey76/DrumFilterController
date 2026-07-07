#pragma once

#include <stdint.h>

enum class SystemState {
  BOOT,
  AUTO_WAIT,
  WASH_AUTO,
  POST_WASH,
  SAFETY_PAUSE,
  RETRY_PAUSE,
  MANUAL,
  MAINTENANCE,
  TEST_WASH,
  DEGRADED,
  FAULT
};

struct InputsSnapshot {
  bool epLavage = false;
  bool epCritique = false;
  bool capotOuvert = false;

  bool modeAuto = true;
  bool modeMaintenance = false;

  bool btnReset = false;
  bool btnTestLavage = false;
  bool btnManuTambour = false;
  bool btnManuRincage = false;

  bool tempBassinValid = true;
  bool tempLocalValid = true;
  float tempBassinC = 20.0F;
  float tempLocalC = 20.0F;
};

struct OutputsCommand {
  bool cmdTambour = false;
  bool cmdRincage = false;
  bool cmdPompeFiltration = false;
  bool cmdPompeDeco = false;
  bool cmdUv = false;
  bool cmdMiseANiveau = false;

  bool voyantMarche = false;
  bool voyantLavage = false;
  bool voyantAlarme = false;
};

struct ControllerStatus {
  SystemState state = SystemState::BOOT;
  const char* message = "BOOT";
  const char* alarmCode = nullptr;
};
