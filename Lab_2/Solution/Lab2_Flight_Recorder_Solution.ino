/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 2.1: The Flight Recorder (Solution Key with Testbench)
 * Objective: Read, Erase, and Write boot counts using Adafruit_SPIFlash.
 * Concept: Flash memory retains data between reboots to act as a flight recorder.
 * REMEMBER: In Flash memory, you MUST erase a sector before writing new data!
 */

#include <SPI.h>
#include "SdFat_Adafruit_Fork.h"

#define EXTERNAL_FLASH_USE_SPI 1
#include <Adafruit_SPIFlash.h>

// ====================================================================
// HARDWARE PIN DEFINITIONS (Pre-configured for FlatSat OBC)
// ====================================================================
SPIClass SD_SPI(PB15, PB14, PB13);
Adafruit_FlashTransport_SPI flashTransport(PB12, SD_SPI);
Adafruit_SPIFlash flash(&flashTransport);

uint8_t currentBootCount = 0;

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n=== FlatSat Flight Recorder Booting ===");

  if (!flash.begin()) {
    Serial.println("❌ CRITICAL ERROR: Hardware not responding!");
    while (1); 
  }

  // =========================================================
  // 🐛 BIST PREPARATION (DO NOT MODIFY)
  // We intentionally inject a "dirty byte" at address 0x0001. 
  // If you skip the erase step, Testcase 3 will catch you!
  // =========================================================
  uint8_t dirtyByte = 0x00;
  flash.writeBuffer(0x0001, &dirtyByte, 1);
  // =========================================================

  // ---------------------------------------------------------
  // TODO: Flash Initialization & Flight Recorder Logic
  // ---------------------------------------------------------
  
  uint32_t jedecId = 0; 
  
  // TODO 1 (FILLED): Read the JEDEC ID
  jedecId = flash.getJEDECID();

  Serial.print("SPI Flash JEDEC ID: 0x");
  Serial.println(jedecId, HEX);


  uint8_t readValue = 0;

  // TODO 2 (FILLED): Read 1 byte of data from Flash address 0x0000
  flash.readBuffer(0x0000, &readValue, 1);

  // TODO 3 (FILLED): Check if Flash is empty (0xFF) or has previous data.
  if (readValue == 0xFF) {
    currentBootCount = 0;
  } else {
    currentBootCount = readValue + 1;
  }

  Serial.print("System is booting for the [ ");
  Serial.print(currentBootCount);
  Serial.println(" ] time(s).");


  // TODO 4 (FILLED): Rule of Flash memory -> Erase the sector before writing!
  // Erasing sector 0 clears both our data (0x0000) and the injected dirty byte (0x0001)
  flash.eraseSector(0);

  // TODO 5 (FILLED): Save the updated 'currentBootCount' back to Flash
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

  // Test 1: Hardware Identification Check (Verifies TODO 1)
  if (id == 0 || id == 0xFFFFFF) {
    Serial.println("❌ [FAIL] Hardware Check: Invalid JEDEC ID. Did you complete TODO 1?");
    isPassed = false;
  } else {
    Serial.println("✅ [PASS] Hardware Check: SPI Flash is responding.");
  }

  // Test 2: Data Integrity Check (Verifies TODO 5 & TODO 3)
  uint8_t verifyCount = 0;
  flash.readBuffer(0x0000, &verifyCount, 1);
  
  if (verifyCount != expectedCount) {
    Serial.println("❌ [FAIL] Integrity Check: Expected boot count " + String(expectedCount) + " but read " + String(verifyCount));
    Serial.println("   * Hint: Did you write the variable back to address 0x0000 in TODO 5?");
    isPassed = false;
  } else {
    Serial.println("✅ [PASS] Integrity Check: Boot count successfully verified in Flash.");
  }

  // Test 3: Erase Operation Check (Verifies TODO 4)
  uint8_t verifyErase = 0;
  flash.readBuffer(0x0001, &verifyErase, 1);
  
  if (verifyErase != 0xFF) {
    Serial.println("❌ [FAIL] Erase Check: Adjacent byte at 0x0001 is NOT clean (0xFF)!");
    Serial.println("   * Hint: You MUST call flash.eraseSector(0) before writing in TODO 4!");
    isPassed = false;
  } else {
    Serial.println("✅ [PASS] Erase Check: Sector 0 was properly formatted.");
  }

  // Final Conclusion
  if (isPassed) {
    Serial.println("🌟 [SYSTEM READY] Flight Recorder subsystem is fully operational.");
  } else {
    Serial.println("⚠️ [SYSTEM WARNING] Flight Recorder malfunction. Data risk imminent.");
  }
  Serial.println("-------------------------------------------\n");
}