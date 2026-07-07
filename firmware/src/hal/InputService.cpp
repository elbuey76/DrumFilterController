#include "InputService.h"

void InputService::begin() {}

InputsSnapshot InputService::read(unsigned long /*nowMs*/) {
#if USE_SIM_INPUTS
  return simulatedInputs_;
#else
  InputsSnapshot inputs{};
  return inputs;
#endif
}

InputsSnapshot& InputService::simulatedInputs() {
  return simulatedInputs_;
}
