#pragma once

#include <Arduino.h>

#include "app/Types.h"

class DisplayService {
public:
  void begin(Stream& stream);
  void render(const ControllerStatus& status, const InputsSnapshot& inputs, const OutputsCommand& outputs);

private:
  const char* stateName(SystemState state) const;
  const char* modeName(const InputsSnapshot& inputs, SystemState state) const;
  const char* levelName(const InputsSnapshot& inputs) const;
  const char* boolText(bool value) const;
  bool hasChanged(const ControllerStatus& status, const InputsSnapshot& inputs, const OutputsCommand& outputs) const;

  Stream* stream_ = nullptr;
  ControllerStatus lastStatus_{};
  InputsSnapshot lastInputs_{};
  OutputsCommand lastOutputs_{};
  bool firstRender_ = true;
};
