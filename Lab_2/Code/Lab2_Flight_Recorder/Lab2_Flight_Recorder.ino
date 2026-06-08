/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 2.1: The Flight Recorder
 */

#include <SPI.h>
#include "SdFat_Adafruit_Fork.h"

#define EXTERNAL_FLASH_USE_SPI 1
#include <Adafruit_SPIFlash.h>  

#include "Lab2_TB_Flight_recorder.h"

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
    Serial.println(" 🔴 CRITICAL ERROR: SPI Flash Hardware not responding!");
    while (1);
  }

  // ====================================================================
  // TB PREPARATION (DO NOT MODIFY)
  // ====================================================================
  uint8_t dirtyByte = 0x00;
  flash.writeBuffer(0x0001, &dirtyByte, 1);
  // ====================================================================

  // ---------------------------------------------------------
  // TODO: Flash Initialization & Flight Recorder Logic
  // ---------------------------------------------------------
  
  uint32_t jedecId = 0; 
  uint8_t currentBootCount = 0;
  
  // TODO 1: Read the JEDEC ID from the Flash memory
  // Hint: Use flash.getJEDECID();
  // [Add your code here]

  Serial.print("SPI Flash JEDEC ID: 0x");
  Serial.println(jedecId, HEX);

  uint8_t readValue = 0;

  // TODO 2: Read 1 byte of data from Flash address 0x0000
  // Hint: Use flash.readBuffer(address, pointer_to_variable, size);
  // [Add your code here]

  // TODO 3: Check if Flash is empty (0xFF) or has previous data.
  // - If 'readValue' is 0xFF (empty), set 'currentBootCount' to 0.
  // - Otherwise, set 'currentBootCount' to readValue + 1.
  // [Add your logic here]

  Serial.print("System is booting for the [ ");
  Serial.print(currentBootCount);
  Serial.println(" ] time(s).");

  // TODO 4: Rule of Flash memory -> Erase the sector before writing!
  // Erase Sector 0 (which contains address 0x0000 and the dirty byte at 0x0001).
  // Hint: Use flash.eraseSector(sector_number);
  // [Add your code here]

  // TODO 5: Save the updated 'currentBootCount' back to Flash at address 0x0000.
  // Hint: Use flash.writeBuffer(address, pointer_to_variable, size);
  // [Add your code here]

  // ---------------------------------------------------------
  // Run Built-In Self-Test (BIST)
  // ---------------------------------------------------------
  runFlightRecorderTestbench(flash, jedecId, currentBootCount);
}

void loop() {
  // Flight recorder updates only occur once per boot sequence
}