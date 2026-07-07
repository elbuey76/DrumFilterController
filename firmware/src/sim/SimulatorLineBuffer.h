#pragma once

#include <stddef.h>

enum class SimulatorLineEvent {
  NONE,
  LINE_READY,
  BUFFER_OVERFLOW
};

class SimulatorLineBuffer {
public:
  SimulatorLineBuffer(char* buffer, size_t bufferSize);

  void reset();
  SimulatorLineEvent push(char received);
  const char* line() const;
  size_t length() const;

private:
  char* buffer_;
  size_t bufferSize_;
  size_t length_ = 0;
};
