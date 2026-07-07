#include "PreferencesPersistentStore.h"

namespace {
const char* keyForEvent(PersistentEventCode eventCode) {
  switch (eventCode) {
    case PersistentEventCode::BOOT: return "boot";
    case PersistentEventCode::A01: return "a01";
    case PersistentEventCode::A02: return "a02";
    case PersistentEventCode::A03: return "a03";
    case PersistentEventCode::A04: return "a04";
    case PersistentEventCode::A15: return "a15cnt";
    case PersistentEventCode::RESET_OK: return "reset";
    case PersistentEventCode::NONE:
    case PersistentEventCode::COUNT:
      return "";
  }

  return "";
}
}  // namespace

void PreferencesPersistentStore::begin() {
#if defined(ARDUINO_ARCH_ESP32)
  available_ = preferences_.begin("fat-v01", false);
  if (available_) {
    snapshot_ = load();
  }
#else
  available_ = false;
#endif
}

PersistentJournalSnapshot PreferencesPersistentStore::load() {
#if defined(ARDUINO_ARCH_ESP32)
  if (!available_) {
    return snapshot_;
  }

  PersistentJournalSnapshot loaded;
  loaded.totalEvents = preferences_.getUInt("total", 0);
  loaded.lastEvent = static_cast<PersistentEventCode>(preferences_.getUChar("last", static_cast<uint8_t>(PersistentEventCode::NONE)));
  loaded.a15Active = preferences_.getBool("a15act", false);

  for (size_t index = 1; index < kPersistentEventCount; ++index) {
    const PersistentEventCode eventCode = static_cast<PersistentEventCode>(index);
    loaded.eventCounts[index] = preferences_.getUInt(keyForEvent(eventCode), 0);
  }

  snapshot_ = loaded;
#endif
  return snapshot_;
}

void PreferencesPersistentStore::save(const PersistentJournalSnapshot& snapshot) {
  snapshot_ = snapshot;

#if defined(ARDUINO_ARCH_ESP32)
  if (!available_) {
    return;
  }

  preferences_.putUInt("total", snapshot_.totalEvents);
  preferences_.putUChar("last", static_cast<uint8_t>(snapshot_.lastEvent));
  preferences_.putBool("a15act", snapshot_.a15Active);

  for (size_t index = 1; index < kPersistentEventCount; ++index) {
    const PersistentEventCode eventCode = static_cast<PersistentEventCode>(index);
    preferences_.putUInt(keyForEvent(eventCode), snapshot_.eventCounts[index]);
  }
#endif
}

bool PreferencesPersistentStore::available() const {
  return available_;
}
