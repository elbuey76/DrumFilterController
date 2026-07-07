#include <Arduino.h>

#include "app/Config.h"
#include "app/Controller.h"
#include "app/Types.h"
#include "hal/DisplayService.h"
#include "hal/InputService.h"
#include "hal/OutputService.h"
#include "sim/SerialSimulator.h"

namespace {
Config config;
Controller controller(config);
InputsSnapshot inputs;
OutputsCommand outputs;
DisplayService display;
InputService inputService;
OutputService outputService;
SerialSimulator simulator;
}  // namespace

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  controller.begin(millis());
  inputService.begin();
  outputService.begin();
  display.begin(Serial);
  simulator.begin(Serial);
  Serial.println("FAT Controller V0.1");
  display.render(controller.status(), inputs, outputs);
}

void loop() {
  const unsigned long nowMs = millis();

  simulator.poll(inputService.simulatedInputs(), controller, outputs);
  inputs = inputService.read(nowMs);
  outputs = controller.update(inputs, nowMs);
  outputService.apply(outputs);

  if (inputService.simulatedInputs().btnReset) {
    inputService.simulatedInputs().btnReset = false;
  }
  if (inputService.simulatedInputs().btnTestLavage) {
    inputService.simulatedInputs().btnTestLavage = false;
  }

  display.render(controller.status(), inputs, outputService.lastApplied());
}
