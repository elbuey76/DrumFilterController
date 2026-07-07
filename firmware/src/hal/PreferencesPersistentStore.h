#pragma once

#include "app/PersistentStore.h"

#if defined(ARDUINO_ARCH_ESP32)
#include <Preferences.h>
#endif

class PreferencesPersistentStore : public PersistentStore {
public:
  void begin() override;
  PersistentJournalSnapshot load() override;
  void save(const PersistentJournalSnapshot& snapshot) override;
  bool available() const override;

private:
  bool available_ = false;
  PersistentJournalSnapshot snapshot_{};

#if defined(ARDUINO_ARCH_ESP32)
  Preferences preferences_;
#endif
};
