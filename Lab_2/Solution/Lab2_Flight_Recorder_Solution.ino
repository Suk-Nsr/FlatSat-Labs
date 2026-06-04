/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 2.1: The Flight Recorder (Solution Key)
 * Objective: Read, Erase, and Write boot counts using Adafruit_SPIFlash.
 * Concept: Flash memory retains data between reboots to act as a flight recorder.
 */

#include <SPI.h>
#include "SdFat_Adafruit_Fork.h"

// Define that we are using SPI (not QSPI)
#define EXTERNAL_FLASH_USE_SPI 1
#include <Adafruit_SPIFlash.h>

// ====================================================================
// ⚠️ HARDWARE PIN DEFINITIONS ⚠️
// Based on the FlatSat OBC_Flash architecture
// ====================================================================
SPIClass SD_SPI(PB15, PB14, PB13);
Adafruit_FlashTransport_SPI flashTransport(PB12, SD_SPI);
Adafruit_SPIFlash flash(&flashTransport);

uint8_t currentBootCount = 0;

void setup() {
  // UART setup for FlatSat OBC
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n=== FlatSat Flight Recorder Booting ===");

  // Initialize the SPI Flash
  if (!flash.begin()) {
    Serial.println("❌ CRITICAL ERROR: Hardware not responding!");
    Serial.println("   Please check your SPI wiring and connections.");
    while (1); // Halt system
  }

  // Verify hardware connection
  uint32_t jedecId = flash.getJEDECID();
  Serial.print("SPI Flash JEDEC ID: 0x");
  Serial.println(jedecId, HEX);

  // ---------------------------------------------------------
  // TODO: Flight Recorder Logic (FILLED SOLUTION)
  // ---------------------------------------------------------
  
  // 1. Read previous boot count from Sector 0, Address 0x0000
  // Note: Adafruit uses readBuffer which requires a pointer (&)
  uint8_t readValue = 0;
  flash.readBuffer(0x0000, &readValue, 1);

  // 2. Check if Flash is empty (0xFF) or has previous data
  if (readValue == 0xFF) {
    currentBootCount = 0;
  } else {
    currentBootCount = readValue + 1;
  }

  Serial.print("System is booting for the [ ");
  Serial.print(currentBootCount);
  Serial.println(" ] time(s).");

  // 3. Rule of Flash memory: Erase sector before writing new data
  // Note: Adafruit library eraseSector() takes Sector Number (0), not exact address.
  flash.eraseSector(0);

  // 4. Save the updated boot count back to Flash
  flash.writeBuffer(0x0000, &currentBootCount, 1);

  // ---------------------------------------------------------
  // Run Built-In Self-Test (BIST)
  // ---------------------------------------------------------
  runFlightRecorderTestbench(currentBootCount, jedecId);
}

void loop() {
  // Flight recorder updates only occur once per boot sequence
}

// ====================================================================
// 🛑 TESTBENCH SECTION - DO NOT MODIFY THIS CODE 🛑
// This function verifies Flash operations (Read, Erase, Write).
// ====================================================================
void runFlightRecorderTestbench(uint8_t expectedCount, uint32_t id) {
  Serial.println("\n--- Running BIST (Flight Recorder Test) ---");
  bool isPassed = true;

  // Test 1: Hardware Identification Check
  if (id == 0 || id == 0xFFFFFF) {
    Serial.println("❌ [FAIL] Hardware Check: Invalid JEDEC ID. Check wiring.");
    isPassed = false;
  } else {
    Serial.println("✅ [PASS] Hardware Check: SPI Flash is responding.");
  }

  // Test 2: Data Integrity Check
  uint8_t verifyCount = 0;
  flash.readBuffer(0x0000, &verifyCount, 1);
  
  if (verifyCount != expectedCount) {
    Serial.println("❌ [FAIL] Integrity Check: Expected boot count " + String(expectedCount) + " but read " + String(verifyCount));
    Serial.println("   * Hint: Did you write the variable back to address 0x0000?");
    isPassed = false;
  } else {
    Serial.println("✅ [PASS] Integrity Check: Boot count successfully verified in Flash.");
  }

  // Test 3: Erase Operation Check
  uint8_t verifyErase = 0;
  flash.readBuffer(0x0001, &verifyErase, 1);
  
  if (verifyErase != 0xFF) {
    Serial.println("❌ [FAIL] Erase Check: Adjacent byte at 0x0001 is not 0xFF.");
    Serial.println("   * Hint: You MUST call flash.eraseSector(0) before writing!");
    isPassed = false;
  } else {
    Serial.println("✅ [PASS] Erase Check: Sector 0 was properly formatted.");
  }

  if (isPassed) {
    Serial.println("🌟 [SYSTEM READY] Flight Recorder subsystem is fully operational.");
  } else {
    Serial.println("⚠️ [SYSTEM WARNING] Flight Recorder malfunction. Data risk imminent.");
  }
  Serial.println("-------------------------------------------\n");
}