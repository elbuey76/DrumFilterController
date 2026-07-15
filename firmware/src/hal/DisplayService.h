#pragma once

#include <Arduino.h>

#include "app/Types.h"

#ifndef USE_A16_AUXILIARIES
#define USE_A16_AUXILIARIES 0
#endif

class LCDi2c;

class DisplayService {
public:
  void begin(Stream& stream, LCDi2c* lcd = nullptr);
  void render(const ControllerStatus& status, const InputsSnapshot& inputs, const OutputsCommand& outputs);
  bool lcdAvailable() const;

private:
  const char* stateName(SystemState state) const;
  const char* modeName(const InputsSnapshot& inputs, SystemState state) const;
  const char* levelName(const InputsSnapshot& inputs) const;
  const char* boolText(bool value) const;
  bool hasChanged(const ControllerStatus& status, const InputsSnapshot& inputs, const OutputsCommand& outputs) const;
  void renderLcd(const ControllerStatus& status, const InputsSnapshot& inputs, const OutputsCommand& outputs);
  void writeLcdLine(uint8_t row, const char* text);

  Stream* stream_ = nullptr;
  ControllerStatus lastStatus_{};
  InputsSnapshot lastInputs_{};
  OutputsCommand lastOutputs_{};
  bool firstRender_ = true;
#if USE_A16_AUXILIARIES
  LCDi2c* lcd_ = nullptr;
#endif
};
