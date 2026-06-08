/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 2.1: The Flight Recorder
 */

#include <SPI.h>
#include "SdFat_Adafruit_Fork.h"

#define EXTERNAL_FLASH_USE_SPI 1
#include <Adafruit_SPIFlash.h>  

#include "src/Lab2_TB_Flight_Recorder.h"

// ====================================================================
// HARDWARE PIN DEFINITIONS (Pre-configured for FlatSat OBC)
// ====================================================================
SPIClass SD_SPI(PB15, PB14, PB13);
Adafruit_FlashTransport_SPI flashTransport(PB12, SD_SPI);
Adafruit_SPIFlash flash(&flashTransport);

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
  // 🐛 FAULT INJECTION (Don't edit this section)
  // ====================================================================
  uint8_t dirtyByte = 0x00;
  flash.writeBuffer(0x0001, &dirtyByte, 1);
  // ====================================================================

  // ---------------------------------------------------------
  // TODO: Flash Initialization & Flight Recorder Logic
  // ---------------------------------------------------------
  
  uint32_t jedecId = 0; 
  uint8_t currentBootCount = 0;
  
  // TODO 1 (FILLED): Read the JEDEC ID from the Flash memory

  Serial.print("SPI Flash JEDEC ID: 0x");
  Serial.println(jedecId, HEX);


  uint8_t readValue = 0;

  // TODO 2 (FILLED): Read 1 byte of data from Flash address 0x0000

  // TODO 3 (FILLED): Check if Flash is empty (0xFF) or has previous data.

  Serial.print("System is booting for the [ ");
  Serial.print(currentBootCount);
  Serial.println(" ] time(s).");

  // TODO 4 (FILLED): Rule of Flash memory -> Erase the sector before writing!

  // TODO 5 (FILLED): Save the updated 'currentBootCount' back to Flash at address 0x0000.

  // ---------------------------------------------------------
  // Run Built-In Self-Test (BIST)
  // ---------------------------------------------------------
  runFlightRecorderTestbench(flash, jedecId, currentBootCount);
}

void loop() {
  // Flight recorder updates only occur once per boot sequence
}