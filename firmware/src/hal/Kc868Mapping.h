#pragma once

#include <stddef.h>
#include <stdint.h>

#include "app/Types.h"

constexpr size_t kKc868DigitalBankCount = 4;
constexpr size_t kKc868DigitalBitsPerBank = 8;
constexpr size_t kKc868DigitalInputCount = 9;
constexpr size_t kKc868DigitalOutputCount = 9;

enum class Kc868InputSignal : uint8_t {
  EP_LAVAGE = 0,
  EP_CRITIQUE,
  CAPOT_OUVERT,
  MODE_AUTO,
  MODE_MAINTENANCE,
  RESET_ALARME,
  TEST_LAVAGE,
  MANU_TAMBOUR,
  MANU_RINCAGE
};

enum class Kc868OutputSignal : uint8_t {
  CMD_TAMBOUR = 0,
  CMD_RINCAGE,
  CMD_POMPE_FILTRATION,
  CMD_POMPE_DECO,
  CMD_UV,
  CMD_MISE_A_NIVEAU,
  VOYANT_MARCHE,
  VOYANT_LAVAGE,
  VOYANT_ALARME
};

struct Kc868SignalPoint {
  uint8_t bank;
  uint8_t bit;
  bool activeLow;
};

struct Kc868DigitalInputsRaw {
  uint8_t banks[kKc868DigitalBankCount] = {0xFF, 0xFF, 0xFF, 0xFF};
  bool bankOk[kKc868DigitalBankCount] = {false, false, false, false};
};

struct Kc868DigitalOutputsRaw {
  uint8_t banks[kKc868DigitalBankCount] = {0xFF, 0xFF, 0xFF, 0xFF};
};

struct Kc868DigitalMapping {
  Kc868SignalPoint inputs[kKc868DigitalInputCount];
  Kc868SignalPoint outputs[kKc868DigitalOutputCount];
};

const Kc868DigitalMapping& defaultKc868DigitalMapping();

bool kc868AllInputBanksOk(const Kc868DigitalInputsRaw& raw);
bool kc868RawSignalActive(uint8_t bankValue, const Kc868SignalPoint& point);
InputsSnapshot kc868MapInputs(const Kc868DigitalInputsRaw& raw, const Kc868DigitalMapping& mapping = defaultKc868DigitalMapping());
OutputsCommand kc868EffectiveOutputs(const OutputsCommand& requested, bool hardwareOutputsArmed, bool hardwareIoHealthy);
Kc868DigitalOutputsRaw kc868MapOutputs(const OutputsCommand& outputs, const Kc868DigitalMapping& mapping = defaultKc868DigitalMapping());
Kc868DigitalOutputsRaw kc868AllOutputsOff(const Kc868DigitalMapping& mapping = defaultKc868DigitalMapping());

const char* kc868InputSignalName(Kc868InputSignal signal);
const char* kc868OutputSignalName(Kc868OutputSignal signal);
