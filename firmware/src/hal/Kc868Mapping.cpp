#include "Kc868Mapping.h"

namespace {
constexpr Kc868SignalPoint signalPoint(uint8_t index, bool activeLow = true) {
  return Kc868SignalPoint{static_cast<uint8_t>(index / kKc868DigitalBitsPerBank),
                          static_cast<uint8_t>(index % kKc868DigitalBitsPerBank),
                          activeLow};
}

bool outputSignal(const OutputsCommand& outputs, Kc868OutputSignal signal) {
  switch (signal) {
    case Kc868OutputSignal::CMD_TAMBOUR: return outputs.cmdTambour;
    case Kc868OutputSignal::CMD_RINCAGE: return outputs.cmdRincage;
    case Kc868OutputSignal::CMD_POMPE_FILTRATION: return outputs.cmdPompeFiltration;
    case Kc868OutputSignal::CMD_POMPE_DECO: return outputs.cmdPompeDeco;
    case Kc868OutputSignal::CMD_UV: return outputs.cmdUv;
    case Kc868OutputSignal::CMD_MISE_A_NIVEAU: return outputs.cmdMiseANiveau;
    case Kc868OutputSignal::VOYANT_MARCHE: return outputs.voyantMarche;
    case Kc868OutputSignal::VOYANT_LAVAGE: return outputs.voyantLavage;
    case Kc868OutputSignal::VOYANT_ALARME: return outputs.voyantAlarme;
  }
  return false;
}

void setMappedOutput(Kc868DigitalOutputsRaw& raw, const Kc868SignalPoint& point, bool active) {
  const uint8_t mask = static_cast<uint8_t>(1U << point.bit);
  const bool rawHigh = point.activeLow ? !active : active;
  if (rawHigh) {
    raw.banks[point.bank] = static_cast<uint8_t>(raw.banks[point.bank] | mask);
  } else {
    raw.banks[point.bank] = static_cast<uint8_t>(raw.banks[point.bank] & static_cast<uint8_t>(~mask));
  }
}
}  // namespace

const Kc868DigitalMapping& defaultKc868DigitalMapping() {
  static const Kc868DigitalMapping mapping = {
      {
          signalPoint(0),  // D1 EP_LAVAGE
          signalPoint(1),  // D2 EP_CRITIQUE
          signalPoint(2),  // D3 CAPOT_OUVERT
          signalPoint(3),  // D4 MODE_AUTO
          signalPoint(4),  // D5 MODE_MAINTENANCE
          signalPoint(5),  // D6 RESET_ALARME
          signalPoint(6),  // D7 TEST_LAVAGE
          signalPoint(7),  // D8 MANU_TAMBOUR
          signalPoint(8),  // D9 MANU_RINCAGE
      },
      {
          signalPoint(0),  // R1 CMD_TAMBOUR
          signalPoint(1),  // R2 CMD_RINCAGE
          signalPoint(2),  // R3 CMD_POMPE_FILTRATION
          signalPoint(3),  // R4 CMD_POMPE_DECO
          signalPoint(4),  // R5 CMD_UV
          signalPoint(5),  // R6 CMD_MISE_A_NIVEAU
          signalPoint(6),  // R7 VOYANT_MARCHE
          signalPoint(7),  // R8 VOYANT_LAVAGE
          signalPoint(8),  // R9 VOYANT_ALARME
      }};
  return mapping;
}

bool kc868AllInputBanksOk(const Kc868DigitalInputsRaw& raw) {
  for (size_t index = 0; index < kKc868DigitalBankCount; ++index) {
    if (!raw.bankOk[index]) {
      return false;
    }
  }
  return true;
}

bool kc868RawSignalActive(uint8_t bankValue, const Kc868SignalPoint& point) {
  const bool rawHigh = (bankValue & static_cast<uint8_t>(1U << point.bit)) != 0;
  return point.activeLow ? !rawHigh : rawHigh;
}

InputsSnapshot kc868MapInputs(const Kc868DigitalInputsRaw& raw, const Kc868DigitalMapping& mapping) {
  InputsSnapshot inputs;

  if (!kc868AllInputBanksOk(raw)) {
    inputs.modeAuto = false;
    inputs.modeMaintenance = true;
    inputs.capotOuvert = true;
    return inputs;
  }

  const auto active = [&](Kc868InputSignal signal) {
    const Kc868SignalPoint& point = mapping.inputs[static_cast<uint8_t>(signal)];
    return kc868RawSignalActive(raw.banks[point.bank], point);
  };

  inputs.epLavage = active(Kc868InputSignal::EP_LAVAGE);
  inputs.epCritique = active(Kc868InputSignal::EP_CRITIQUE);
  inputs.capotOuvert = active(Kc868InputSignal::CAPOT_OUVERT);
  inputs.modeAuto = active(Kc868InputSignal::MODE_AUTO);
  inputs.modeMaintenance = active(Kc868InputSignal::MODE_MAINTENANCE);
  inputs.btnReset = active(Kc868InputSignal::RESET_ALARME);
  inputs.btnTestLavage = active(Kc868InputSignal::TEST_LAVAGE);
  inputs.btnManuTambour = active(Kc868InputSignal::MANU_TAMBOUR);
  inputs.btnManuRincage = active(Kc868InputSignal::MANU_RINCAGE);

  inputs.tempBassinValid = true;
  inputs.tempLocalValid = true;
  inputs.tempBassinC = 20.0F;
  inputs.tempLocalC = 20.0F;
  return inputs;
}

OutputsCommand kc868EffectiveOutputs(const OutputsCommand& requested, bool hardwareOutputsArmed, bool hardwareIoHealthy) {
  if (!hardwareOutputsArmed || !hardwareIoHealthy) {
    return OutputsCommand{};
  }
  return requested;
}

Kc868DigitalOutputsRaw kc868MapOutputs(const OutputsCommand& outputs, const Kc868DigitalMapping& mapping) {
  Kc868DigitalOutputsRaw raw = kc868AllOutputsOff(mapping);
  for (uint8_t index = 0; index < kKc868DigitalOutputCount; ++index) {
    const auto signal = static_cast<Kc868OutputSignal>(index);
    setMappedOutput(raw, mapping.outputs[index], outputSignal(outputs, signal));
  }
  return raw;
}

Kc868DigitalOutputsRaw kc868AllOutputsOff(const Kc868DigitalMapping& mapping) {
  Kc868DigitalOutputsRaw raw;
  for (uint8_t index = 0; index < kKc868DigitalOutputCount; ++index) {
    setMappedOutput(raw, mapping.outputs[index], false);
  }
  return raw;
}

const char* kc868InputSignalName(Kc868InputSignal signal) {
  switch (signal) {
    case Kc868InputSignal::EP_LAVAGE: return "EP_LAVAGE";
    case Kc868InputSignal::EP_CRITIQUE: return "EP_CRITIQUE";
    case Kc868InputSignal::CAPOT_OUVERT: return "CAPOT_OUVERT";
    case Kc868InputSignal::MODE_AUTO: return "MODE_AUTO";
    case Kc868InputSignal::MODE_MAINTENANCE: return "MODE_MAINTENANCE";
    case Kc868InputSignal::RESET_ALARME: return "RESET_ALARME";
    case Kc868InputSignal::TEST_LAVAGE: return "TEST_LAVAGE";
    case Kc868InputSignal::MANU_TAMBOUR: return "MANU_TAMBOUR";
    case Kc868InputSignal::MANU_RINCAGE: return "MANU_RINCAGE";
  }
  return "?";
}

const char* kc868OutputSignalName(Kc868OutputSignal signal) {
  switch (signal) {
    case Kc868OutputSignal::CMD_TAMBOUR: return "CMD_TAMBOUR";
    case Kc868OutputSignal::CMD_RINCAGE: return "CMD_RINCAGE";
    case Kc868OutputSignal::CMD_POMPE_FILTRATION: return "CMD_POMPE_FILTRATION";
    case Kc868OutputSignal::CMD_POMPE_DECO: return "CMD_POMPE_DECO";
    case Kc868OutputSignal::CMD_UV: return "CMD_UV";
    case Kc868OutputSignal::CMD_MISE_A_NIVEAU: return "CMD_MISE_A_NIVEAU";
    case Kc868OutputSignal::VOYANT_MARCHE: return "VOYANT_MARCHE";
    case Kc868OutputSignal::VOYANT_LAVAGE: return "VOYANT_LAVAGE";
    case Kc868OutputSignal::VOYANT_ALARME: return "VOYANT_ALARME";
  }
  return "?";
}
