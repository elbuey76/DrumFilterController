#include "InputService.h"

#if USE_A16_AUXILIARIES
#include "hal/TemperatureService.h"
#endif

InputService::InputService(Kc868Pcf8574Io* kc868Io, TemperatureService* temperatureService) {
#if USE_KC868_IO
  kc868Io_ = kc868Io;
#else
  (void)kc868Io;
#endif
#if USE_A16_AUXILIARIES
  temperatureService_ = temperatureService;
#else
  (void)temperatureService;
#endif
}

void InputService::begin() {
#if USE_KC868_IO
  if (kc868Io_ != nullptr) {
    kc868Io_->begin();
  }
#endif
#if USE_A16_AUXILIARIES
  if (temperatureService_ != nullptr) {
    temperatureService_->begin();
  }
#endif
}

InputsSnapshot InputService::read(unsigned long nowMs) {
#if USE_SIM_INPUTS
  hardwareIoHealthy_ = true;
  lastInputs_ = simulatedInputs_;
#else
#if USE_KC868_IO
  if (kc868Io_ == nullptr) {
    hardwareIoHealthy_ = false;
    lastInputs_ = kc868MapInputs(lastHardwareRawInputs_);
    return lastInputs_;
  }
  lastHardwareRawInputs_ = kc868Io_->readInputs();
  hardwareIoHealthy_ = kc868AllInputBanksOk(lastHardwareRawInputs_);
  lastInputs_ = kc868MapInputs(lastHardwareRawInputs_, kc868Io_->profile().mapping);
#if USE_A16_AUXILIARIES
  if (temperatureService_ != nullptr) {
    temperatureService_->poll(nowMs);
    temperatureService_->applyTo(lastInputs_);
  }
#endif
#else
  hardwareIoHealthy_ = false;
  lastInputs_ = InputsSnapshot{};
#endif
#endif
  return lastInputs_;
}

InputsSnapshot& InputService::simulatedInputs() {
  return simulatedInputs_;
}

const InputsSnapshot& InputService::lastInputs() const {
  return lastInputs_;
}

bool InputService::hardwareIoHealthy() const {
  return hardwareIoHealthy_;
}

Kc868DigitalInputsRaw InputService::lastHardwareRawInputs() const {
  return lastHardwareRawInputs_;
}

#if USE_KC868_IO
void InputService::scanI2c(Stream& stream) {
  if (kc868Io_ == nullptr) {
    stream.println(F("I2C interne indisponible."));
    return;
  }

  uint8_t addresses[16]{};
  const size_t count = kc868Io_->scanI2c(addresses, sizeof(addresses));
  stream.println(F("--- I2C INTERNE A16 ---"));
  const size_t printed = count < sizeof(addresses) ? count : sizeof(addresses);
  for (size_t index = 0; index < printed; ++index) {
    stream.print(F("0x"));
    if (addresses[index] < 16) {
      stream.print('0');
    }
    stream.println(addresses[index], HEX);
  }
  stream.print(F("Peripheriques detectes: "));
  stream.println(count);
}

const Kc868A16HardwareProfile& InputService::hardwareProfile() const {
  return kc868Io_ == nullptr ? selectedKc868A16Profile() : kc868Io_->profile();
}
#endif

#if USE_A16_AUXILIARIES
void InputService::printTemperatureDiagnostics(Stream& stream) const {
  if (temperatureService_ == nullptr) {
    stream.println(F("Service DS18B20 indisponible."));
    return;
  }
  temperatureService_->printDiagnostics(stream);
}
#endif
