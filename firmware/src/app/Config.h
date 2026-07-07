#pragma once

#include <stdint.h>

struct Config {
  unsigned long epLavageDelayMs = 10000;
  unsigned long epCritiqueDebounceMs = 1000;

  unsigned long capotOpenDebounceMs = 300;
  unsigned long capotCloseStableMs = 1500;
  unsigned long capotLongOpenMs = 10UL * 60UL * 1000UL;

  unsigned long washMinMs = 10000;
  unsigned long washMaxMs = 45000;
  unsigned long residualRotationMs = 3000;
  unsigned long antiRestartMs = 60000;
  unsigned long retryPauseMs = 60000;

  uint8_t maxWashAttempts = 3;

  float tempBassinLowC = 4.0F;
  float tempBassinHighC = 28.0F;
  float tempLocalLowC = 2.0F;
  float tempLocalHighC = 40.0F;
};
