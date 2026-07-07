#pragma once

#include <Arduino.h>

#include "app/Controller.h"
#include "hal/InputService.h"
#include "hal/OutputService.h"
#include "sim/SimulatorLineBuffer.h"

class Kc868Diagnostics {
public:
  void begin(Stream& stream);
  void poll(InputService& inputService, OutputService& outputService, const InputsSnapshot& inputs, const Controller& controller);

private:
  static constexpr size_t kBufferSize = 96;

  void handleCommand(const String& command, InputService& inputService, OutputService& outputService, const InputsSnapshot& inputs, const Controller& controller);
  void printHelp() const;
  void printInputs(InputService& inputService, const InputsSnapshot& inputs) const;
  void printOutputs(const OutputService& outputService) const;
  void printController(const InputsSnapshot& inputs, const Controller& controller) const;
  void printBool(const char* label, bool value) const;
  void printRawBanks(const char* label, const uint8_t* banks) const;
  bool parseRelayPulse(const String& command, uint8_t& relayNumber, uint16_t& pulseMs) const;

  Stream* stream_ = nullptr;
  char buffer_[kBufferSize]{};
  SimulatorLineBuffer lineBuffer_{buffer_, kBufferSize};
};
