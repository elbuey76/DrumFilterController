#include <Arduino.h>
#include <RTClib.h>
#include <Wire.h>

#ifndef RTC_LCD_TEST
#define RTC_LCD_TEST 0
#endif

#if RTC_LCD_TEST
#include <LCDi2c.h>
#endif

namespace {
constexpr uint8_t kInternalSda = 4;
constexpr uint8_t kInternalScl = 5;
constexpr uint8_t kOutputBankAddresses[] = {0x24, 0x25};

constexpr uint8_t kAuxSda = 32;  // HT1 / GPIO1 on the KC868-A16 terminal.
constexpr uint8_t kAuxScl = 33;  // HT2 / GPIO2 on the KC868-A16 terminal.
constexpr uint8_t kRtcAddress = 0x68;
constexpr uint8_t kLcdAddresses[] = {0x27, 0x3F};

TwoWire& internalWire = Wire;
TwoWire& rtcWire = Wire1;
RTC_DS3231 rtc;

bool rtcPresent = false;
unsigned long lastPrintMs = 0;
String serialLine;
#if RTC_LCD_TEST
bool lcdPresent = false;
LCDi2c lcd27(0x27, rtcWire);
LCDi2c lcd3f(0x3F, rtcWire);
LCDi2c* lcd = nullptr;
#endif

bool writeOutputBankOff(uint8_t address) {
  internalWire.beginTransmission(address);
  internalWire.write(0xFF);
  return internalWire.endTransmission() == 0;
}

void keepOutputsOff() {
  for (const uint8_t address : kOutputBankAddresses) {
    writeOutputBankOff(address);
  }
}

bool ping(TwoWire& wire, uint8_t address) {
  wire.beginTransmission(address);
  return wire.endTransmission() == 0;
}

void printTwoDigits(uint8_t value) {
  if (value < 10) {
    Serial.print('0');
  }
  Serial.print(value);
}

void printDateTime(const DateTime& value) {
  Serial.print(value.year());
  Serial.print('-');
  printTwoDigits(value.month());
  Serial.print('-');
  printTwoDigits(value.day());
  Serial.print(' ');
  printTwoDigits(value.hour());
  Serial.print(':');
  printTwoDigits(value.minute());
  Serial.print(':');
  printTwoDigits(value.second());
}

void printStatus() {
  Serial.println(F("--- RTC DS3231 ---"));
  Serial.print(F("Adresse 0x68 detectee : "));
  Serial.println(ping(rtcWire, kRtcAddress) ? F("OUI") : F("NON"));
#if RTC_LCD_TEST
  Serial.print(F("LCD detecte            : "));
  if (lcdPresent) {
    Serial.println(F("OUI"));
  } else {
    Serial.println(F("NON (0x27 / 0x3F)"));
  }
#endif

  if (!rtcPresent) {
    Serial.println(F("RTC initialisee       : NON"));
    return;
  }

  const DateTime now = rtc.now();
  Serial.println(F("RTC initialisee       : OUI"));
  Serial.print(F("Perte alimentation    : "));
  Serial.println(rtc.lostPower() ? F("OUI") : F("NON"));
  Serial.print(F("Date et heure         : "));
  printDateTime(now);
  Serial.println();
  Serial.print(F("Heure valide          : "));
  Serial.println(now.isValid() ? F("OUI") : F("NON"));
  Serial.print(F("Temperature RTC       : "));
  Serial.print(rtc.getTemperature(), 2);
  Serial.println(F(" deg C"));
}

void scanAuxiliaryI2c() {
  Serial.println(F("--- SCAN I2C HT1/HT2 ---"));
  uint8_t count = 0;
  for (uint8_t address = 1; address < 127; ++address) {
    if (!ping(rtcWire, address)) {
      continue;
    }
    Serial.print(F("Trouve : 0x"));
    if (address < 16) {
      Serial.print('0');
    }
    Serial.println(address, HEX);
    ++count;
  }
  Serial.print(F("Nombre de peripheriques : "));
  Serial.println(count);
}

void printHelp() {
  Serial.println(F("Commandes :"));
  Serial.println(F("  status       affiche l'etat de la RTC"));
  Serial.println(F("  scan         scanne le bus I2C HT1/HT2"));
  Serial.println(F("  set compile  regle la RTC sur la date/heure de compilation"));
  Serial.println(F("  help         affiche cette aide"));
}

#if RTC_LCD_TEST
void writeLcdLine(uint8_t row, const char* text) {
  if (lcd == nullptr) {
    return;
  }

  char padded[21];
  memset(padded, ' ', 20);
  padded[20] = '\0';
  const size_t length = strlen(text);
  memcpy(padded, text, length < 20 ? length : 20);
  lcd->locate(row + 1, 1);
  lcd->print(padded);
}

void renderLcd() {
  if (lcd == nullptr) {
    return;
  }

  writeLcdLine(0, "TEST RTC + LCD");
  writeLcdLine(1, "I2C: RTC 0x68 OK");
  if (!rtcPresent) {
    writeLcdLine(2, "RTC: NON DETECTEE");
    writeLcdLine(3, "Verifier D/C +3V3");
    return;
  }

  char line[21];
  snprintf(line, sizeof(line), "Pile: %s", rtc.lostPower() ? "A REGLER" : "OK");
  writeLcdLine(2, line);

  const DateTime now = rtc.now();
  snprintf(line, sizeof(line), "%04d-%02d-%02d %02d:%02d:%02d", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  writeLcdLine(3, line);
}

void beginLcd() {
  if (ping(rtcWire, 0x27)) {
    lcd = &lcd27;
  } else if (ping(rtcWire, 0x3F)) {
    lcd = &lcd3f;
  }

  if (lcd == nullptr) {
    return;
  }

  lcdPresent = true;
  lcd->begin(4, 20);
  lcd->display(BACKLIGHT_ON);
  renderLcd();
}
#endif

void processCommand(String command) {
  command.trim();
  command.toLowerCase();

  if (command == "status") {
    printStatus();
    return;
  }
  if (command == "scan") {
    scanAuxiliaryI2c();
    return;
  }
  if (command == "set compile") {
    if (!rtcPresent) {
      Serial.println(F("ERREUR : RTC non detectee."));
      return;
    }
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println(F("RTC reglee sur la date et l'heure de compilation."));
    printStatus();
    return;
  }
  if (command == "help" || command.length() == 0) {
    printHelp();
    return;
  }

  Serial.println(F("Commande inconnue."));
  printHelp();
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println(F("=== TEST RTC KC868-A16 REV.1.6.3 ==="));
  Serial.println(F("Sorties Y maintenues OFF. Aucun organe n'est commande."));

  internalWire.begin(kInternalSda, kInternalScl, 100000);
  keepOutputsOff();
  Serial.print(F("Banque Y1-Y8 OFF       : "));
  Serial.println(writeOutputBankOff(kOutputBankAddresses[0]) ? F("OK") : F("ERREUR"));
  Serial.print(F("Banque Y9-Y16 OFF      : "));
  Serial.println(writeOutputBankOff(kOutputBankAddresses[1]) ? F("OK") : F("ERREUR"));

  rtcWire.begin(kAuxSda, kAuxScl, 100000);
  scanAuxiliaryI2c();
  rtcPresent = ping(rtcWire, kRtcAddress) && rtc.begin(&rtcWire);
#if RTC_LCD_TEST
  beginLcd();
#endif
  printStatus();
  printHelp();
}

void loop() {
  keepOutputsOff();

  while (Serial.available() > 0) {
    const char incoming = static_cast<char>(Serial.read());
    if (incoming == '\n' || incoming == '\r') {
      if (serialLine.length() > 0) {
        processCommand(serialLine);
        serialLine = "";
      }
    } else if (serialLine.length() < 80) {
      serialLine += incoming;
    }
  }

  const unsigned long nowMs = millis();
  if (rtcPresent && nowMs - lastPrintMs >= 1000) {
    lastPrintMs = nowMs;
    Serial.print(F("RTC : "));
    printDateTime(rtc.now());
    Serial.println();
#if RTC_LCD_TEST
    renderLcd();
#endif
  }

  delay(10);
}
