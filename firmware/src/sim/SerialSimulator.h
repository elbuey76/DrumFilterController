#pragma once

#include <Arduino.h>

#include "app/Controller.h"
#include "app/Types.h"
#include "sim/SimulatorCommands.h"

class SerialSimulator {
public:
  void begin(Stream& stream);
  void poll(InputsSnapshot& inputs, const Controller& controller, const OutputsCommand& outputs);

private:
  static constexpr size_t kBufferSize = 96;

  void handleCommand(const String& command, InputsSnapshot& inputs, const Controller& controller, const OutputsCommand& outputs);
  void printHelp() const;
  void printStatus(const InputsSnapshot& inputs, const Controller& controller, const OutputsCommand& outputs) const;
  void printInputs(const InputsSnapshot& inputs) const;
  void printOutputs(const OutputsCommand& outputs) const;
  void printState(SystemState state) const;
  void setBool(const char* label, bool value) const;

  Stream* stream_ = nullptr;
  char buffer_[kBufferSize]{};
  size_t bufferLength_ = 0;
};
