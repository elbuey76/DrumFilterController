#include <Arduino.h>

#include "app/Config.h"
#include "app/Controller.h"
#include "app/Types.h"
#include "hal/DisplayService.h"
#include "hal/InputService.h"
#include "hal/OutputService.h"
#include "hal/PreferencesPersistentStore.h"
#if USE_SIM_INPUTS
#include "sim/SerialSimulator.h"
#elif USE_KC868_IO
#include "hal/Kc868Diagnostics.h"
#endif

namespace {
Config config;
PreferencesPersistentStore persistentStore;
Controller controller(config, &persistentStore);
InputsSnapshot inputs;
OutputsCommand outputs;
DisplayService display;
InputService inputService;
OutputService outputService;
#if USE_SIM_INPUTS
SerialSimulator simulator;
#elif USE_KC868_IO
Kc868Diagnostics diagnostics;
#endif
}  // namespace

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  outputService.begin();
  inputService.begin();
  controller.begin(millis());
  display.begin(Serial);
#if USE_SIM_INPUTS
  simulator.begin(Serial);
#elif USE_KC868_IO
  diagnostics.begin(Serial);
#endif
  Serial.println("FAT Controller V0.2");
  display.render(controller.status(), inputs, outputs);
}

void loop() {
  const unsigned long nowMs = millis();

#if USE_SIM_INPUTS
  simulator.poll(inputService.simulatedInputs(), controller, outputs);
#endif
  inputs = inputService.read(nowMs);
#if USE_KC868_IO
  outputService.setHardwareIoHealthy(inputService.hardwareIoHealthy());
#endif
  outputs = controller.update(inputs, nowMs);
  outputService.apply(outputs);

#if USE_SIM_INPUTS
  if (inputService.simulatedInputs().btnReset) {
    inputService.simulatedInputs().btnReset = false;
  }
  if (inputService.simulatedInputs().btnTestLavage) {
    inputService.simulatedInputs().btnTestLavage = false;
  }
#elif USE_KC868_IO
  diagnostics.poll(inputService, outputService, inputs, controller);
#endif

  display.render(controller.status(), inputs, outputService.lastApplied());
}
