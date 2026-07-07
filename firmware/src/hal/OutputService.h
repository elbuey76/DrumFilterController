#pragma once

#include "app/Types.h"

#ifndef USE_KC868_IO
#define USE_KC868_IO 0
#endif

class OutputService {
public:
  void begin();
  void apply(const OutputsCommand& outputs);
  void allOff();
  OutputsCommand lastApplied() const;

private:
  OutputsCommand lastApplied_{};
};
