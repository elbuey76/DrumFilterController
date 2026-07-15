#include <Arduino.h>

#include "app/Config.h"
#include "app/Controller.h"
#include "app/Types.h"
#include "hal/DisplayService.h"
#include "hal/InputService.h"
#include "hal/OutputService.h"
#include "hal/PreferencesPersistentStore.h"
#if USE_KC868_IO
#include "hal/Kc868A16Profile.h"
#include "hal/Kc868Pcf8574Io.h"
#include "hal/Kc868WireI2cBus.h"
#endif
#if USE_A16_AUXILIARIES
#include <LCDi2c.h>
#include <Wire.h>

#include "hal/A16AuxiliaryService.h"
#include "hal/TemperatureService.h"
#endif
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
#if USE_KC868_IO
Kc868WireI2cBus kc868Bus;
Kc868Pcf8574Io kc868Io(kc868Bus, selectedKc868A16Profile());
#if USE_A16_AUXILIARIES
A16AuxiliaryService auxiliaryService(Wire1, selectedKc868A16Profile());
TemperatureService temperatureService(selectedKc868A16Profile());
LCDi2c lcd(selectedKc868A16Profile().auxiliaries.lcdAddress, Wire1);
InputService inputService(&kc868Io, &temperatureService);
#else
InputService inputService(&kc868Io);
#endif
OutputService outputService(&kc868Io);
#else
InputService inputService;
OutputService outputService;
#endif
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
#if USE_A16_AUXILIARIES
  auxiliaryService.begin();
#endif
  inputService.begin();
  controller.begin(millis());
#if USE_A16_AUXILIARIES
  display.begin(Serial, auxiliaryService.lcdPresent() ? &lcd : nullptr);
#else
  display.begin(Serial);
#endif
#if USE_SIM_INPUTS
  simulator.begin(Serial);
#elif USE_KC868_IO
#if USE_A16_AUXILIARIES
  diagnostics.begin(Serial, &auxiliaryService);
#else
  diagnostics.begin(Serial);
#endif
#endif
  Serial.println("FAT Controller V0.2");
  display.render(controller.status(), inputs, outputs);
}

void loop() {
  const unsigned long nowMs = millis();

#if USE_A16_AUXILIARIES
  auxiliaryService.poll(nowMs);
#endif

#if USE_SIM_INPUTS
  simulator.poll(inputService.simulatedInputs(), controller, outputs);
#endif
  inputs = inputService.read(nowMs);
#if USE_KC868_IO
  outputService.setInputHardwareHealthy(inputService.hardwareIoHealthy());
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
