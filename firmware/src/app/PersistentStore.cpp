#include "PersistentStore.h"

void NullPersistentStore::begin() {}

PersistentJournalSnapshot NullPersistentStore::load() {
  return snapshot_;
}

void NullPersistentStore::save(const PersistentJournalSnapshot& snapshot) {
  snapshot_ = snapshot;
}

bool NullPersistentStore::available() const {
  return false;
}

const char* persistentEventCodeText(PersistentEventCode eventCode) {
  switch (eventCode) {
    case PersistentEventCode::BOOT: return "BOOT";
    case PersistentEventCode::A01: return "A01";
    case PersistentEventCode::A02: return "A02";
    case PersistentEventCode::A03: return "A03";
    case PersistentEventCode::A04: return "A04";
    case PersistentEventCode::A15: return "A15";
    case PersistentEventCode::RESET_OK: return "RESET_OK";
    case PersistentEventCode::NONE: return "NONE";
    case PersistentEventCode::COUNT: return "COUNT";
  }

  return "UNKNOWN";
}

size_t persistentEventIndex(PersistentEventCode eventCode) {
  const size_t index = static_cast<size_t>(eventCode);
  return index < kPersistentEventCount ? index : 0;
}
