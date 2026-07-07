#include "OutputService.h"

void OutputService::begin() {
  allOff();
}

void OutputService::apply(const OutputsCommand& outputs) {
#if USE_KC868_IO
  // Hardware relay writes will be added in firmware V0.2 after pin validation.
#endif
  lastApplied_ = outputs;
}

void OutputService::allOff() {
  lastApplied_ = OutputsCommand{};
}

OutputsCommand OutputService::lastApplied() const {
  return lastApplied_;
}
