#pragma once

#include <stddef.h>
#include <stdint.h>

enum class PersistentEventCode : uint8_t {
  NONE = 0,
  BOOT,
  A01,
  A02,
  A03,
  A04,
  A15,
  RESET_OK,
  COUNT
};

constexpr size_t kPersistentEventCount = static_cast<size_t>(PersistentEventCode::COUNT);

struct PersistentJournalSnapshot {
  uint32_t eventCounts[kPersistentEventCount] = {};
  uint32_t totalEvents = 0;
  PersistentEventCode lastEvent = PersistentEventCode::NONE;
  bool a15Active = false;
};

class PersistentStore {
public:
  virtual ~PersistentStore() = default;

  virtual void begin() = 0;
  virtual PersistentJournalSnapshot load() = 0;
  virtual void save(const PersistentJournalSnapshot& snapshot) = 0;
  virtual bool available() const = 0;
};

class NullPersistentStore : public PersistentStore {
public:
  void begin() override;
  PersistentJournalSnapshot load() override;
  void save(const PersistentJournalSnapshot& snapshot) override;
  bool available() const override;

private:
  PersistentJournalSnapshot snapshot_{};
};

const char* persistentEventCodeText(PersistentEventCode eventCode);
size_t persistentEventIndex(PersistentEventCode eventCode);
