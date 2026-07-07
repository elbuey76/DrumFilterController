#include "SimulatorLineBuffer.h"

SimulatorLineBuffer::SimulatorLineBuffer(char* buffer, size_t bufferSize) : buffer_(buffer), bufferSize_(bufferSize) {
  reset();
}

void SimulatorLineBuffer::reset() {
  length_ = 0;
  if (buffer_ != nullptr && bufferSize_ > 0) {
    buffer_[0] = '\0';
  }
}

SimulatorLineEvent SimulatorLineBuffer::push(char received) {
  if (buffer_ == nullptr || bufferSize_ == 0) {
    return SimulatorLineEvent::BUFFER_OVERFLOW;
  }

  if (received == '\r') {
    return SimulatorLineEvent::NONE;
  }

  if (received == '\n') {
    buffer_[length_] = '\0';
    return SimulatorLineEvent::LINE_READY;
  }

  if (length_ < bufferSize_ - 1) {
    buffer_[length_++] = received;
    buffer_[length_] = '\0';
    return SimulatorLineEvent::NONE;
  }

  reset();
  return SimulatorLineEvent::BUFFER_OVERFLOW;
}

const char* SimulatorLineBuffer::line() const {
  return buffer_ == nullptr ? "" : buffer_;
}

size_t SimulatorLineBuffer::length() const {
  return length_;
}
