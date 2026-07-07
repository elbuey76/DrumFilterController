#pragma once

#include "app/Types.h"

#ifndef USE_SIM_INPUTS
#define USE_SIM_INPUTS 1
#endif

#ifndef USE_KC868_IO
#define USE_KC868_IO 0
#endif

class InputService {
public:
  void begin();
  InputsSnapshot read(unsigned long nowMs);
  InputsSnapshot& simulatedInputs();

private:
  InputsSnapshot simulatedInputs_{};
};
