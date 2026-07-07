#include <Arduino.h>

#include "app/Config.h"
#include "app/Controller.h"
#include "app/Types.h"
#include "sim/SerialSimulator.h"

namespace {
Config config;
Controller controller(config);
InputsSnapshot inputs;
OutputsCommand outputs;
SerialSimulator simulator;

const char* stateName(SystemState state) {
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

void printStatus(const ControllerStatus& status, const OutputsCommand& currentOutputs) {
  Serial.print("State: ");
  Serial.println(stateName(status.state));
  Serial.print("Message: ");
  Serial.println(status.message);
  Serial.print("Alarm: ");
  Serial.println(status.alarmCode == nullptr ? "none" : status.alarmCode);
  Serial.print("Tambour: ");
  Serial.println(currentOutputs.cmdTambour ? "ON" : "OFF");
  Serial.print("Rincage: ");
  Serial.println(currentOutputs.cmdRincage ? "ON" : "OFF");
  Serial.print("Filtration: ");
  Serial.println(currentOutputs.cmdPompeFiltration ? "ON" : "OFF");
  Serial.print("UV: ");
  Serial.println(currentOutputs.cmdUv ? "ON" : "OFF");
}
}  // namespace

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  controller.begin(millis());
  simulator.begin(Serial);
  Serial.println("FAT Controller V0.1");
  printStatus(controller.status(), outputs);
}

void loop() {
  static unsigned long lastPrintMs = 0;
  const unsigned long nowMs = millis();

  simulator.poll(inputs, controller, outputs);
  outputs = controller.update(inputs, nowMs);

  if (inputs.btnReset) {
    inputs.btnReset = false;
  }
  if (inputs.btnTestLavage) {
    inputs.btnTestLavage = false;
  }

  if (nowMs - lastPrintMs >= 1000) {
    lastPrintMs = nowMs;
    printStatus(controller.status(), outputs);
  }
}
