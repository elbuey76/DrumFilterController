#include "InputService.h"

void InputService::begin() {
#if USE_KC868_IO
  kc868Io_.begin();
#endif
}

InputsSnapshot InputService::read(unsigned long /*nowMs*/) {
#if USE_SIM_INPUTS
  hardwareIoHealthy_ = true;
  lastInputs_ = simulatedInputs_;
#else
#if USE_KC868_IO
  lastHardwareRawInputs_ = kc868Io_.readInputs();
  hardwareIoHealthy_ = kc868AllInputBanksOk(lastHardwareRawInputs_);
  lastInputs_ = kc868MapInputs(lastHardwareRawInputs_);
#else
  hardwareIoHealthy_ = false;
  lastInputs_ = InputsSnapshot{};
#endif
#endif
  return lastInputs_;
}

InputsSnapshot& InputService::simulatedInputs() {
  return simulatedInputs_;
}

const InputsSnapshot& InputService::lastInputs() const {
  return lastInputs_;
}

bool InputService::hardwareIoHealthy() const {
  return hardwareIoHealthy_;
}

Kc868DigitalInputsRaw InputService::lastHardwareRawInputs() const {
  return lastHardwareRawInputs_;
}

#if USE_KC868_IO
void InputService::scanI2c(Stream& stream) {
  kc868Io_.scanI2c(stream);
}
#endif
