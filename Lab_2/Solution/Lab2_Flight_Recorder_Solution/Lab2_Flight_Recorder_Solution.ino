/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 2.1: The Flight Recorder (Main Architecture File)
 */

#include <SPI.h>
#include "SdFat_Adafruit_Fork.h"

#define EXTERNAL_FLASH_USE_SPI 1
#include <Adafruit_SPIFlash.h>

// --- Hardware Connection Layer ---
SPIClass SD_SPI(PB15, PB14, PB13);
Adafruit_FlashTransport_SPI flashTransport(PB12, SD_SPI);
Adafruit_SPIFlash flash(&flashTransport);

// --- Include Modular Subsystems ---
#include "Lab2_Flight_Recorder_Logic.h"
#include "Lab2_TB_Flight_Recorder.h"

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  while(!Serial){;}

  Serial.println("\n=== FlatSat Flight Recorder Booting ===");

  if (!flash.begin()) {
    Serial.println("❌ CRITICAL ERROR: SPI Flash Hardware not responding!");
    while (1);
  }

  // ====================================================================
  // 🐛 FAULT INJECTION (SABOTAGE)
  // We write a dirty byte (0x00) to 0x0001 BEFORE student code executes.
  // If the student skips the erase sector step, Testcase 3 will catch it!
  // ====================================================================
  uint8_t dirtyByte = 0x00;
  flash.writeBuffer(0x0001, &dirtyByte, 1);
  // ====================================================================

  // Allocate shared parameters to capture student actions
  uint32_t capturedJedecId = 0;
  uint8_t capturedBootCount = 0;

  // Execute student logic workspace (Pass variables by reference)
  executeFlightRecorderLogic(flash, capturedJedecId, capturedBootCount);

  // Run the un-bypassable testbench using variables written BY THE STUDENT
  runFlightRecorderTestbench(flash, capturedJedecId, capturedBootCount);
}

void loop() {
  // Main logic executes only once per boot cycle
}