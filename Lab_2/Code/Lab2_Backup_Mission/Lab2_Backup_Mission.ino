/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 2.3: The Backup Mission (Student Template)
 * Objective: Integrate Dual SPI Buses to transfer data from Flash to SD Card.
 * Scenario: OBC detects a power brownout, copies Boot Count from Flash,
 * and safely creates an emergency file on the SD Card before Deep Sleep.
 */

#include <SPI.h>
#include "SdFat_Adafruit_Fork.h"

// Define that we are using standard SPI (not QSPI) for Flash
#define EXTERNAL_FLASH_USE_SPI 1
#include <Adafruit_SPIFlash.h>

#include "Lab2_TB_Backup_Mission.h"

// ====================================================================
// HARDWARE PIN DEFINITIONS (Dual SPI Bus Architecture)
// FlatSat separates Flash and SD Card onto different SPI buses!
// ====================================================================

// --- SPI FLASH BUS (SPI2) ---
#define FLASH_SCK  PB13  
#define FLASH_MISO PB14  
#define FLASH_MOSI PB15  
#define FLASH_CS   PB12  

SPIClass FLASH_SPI(FLASH_MOSI, FLASH_MISO, FLASH_SCK);
Adafruit_FlashTransport_SPI flashTransport(FLASH_CS, &FLASH_SPI);
Adafruit_SPIFlash flash(&flashTransport);

// --- SD CARD BUS (SPI3) ---
#define SD_SCK  PC10  
#define SD_MISO PC11  
#define SD_MOSI PC12  
#define SD_CS   PC9   

SPIClass SD_SPI(SD_MOSI, SD_MISO, SD_SCK);
SdFat sd;
File backupFile;
const char* textFilename = "safe_boot.txt";

void setup() {
  // UART setup for FlatSat OBC
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  while(!Serial){;}

  Serial.println("\n=== FlatSat Emergency Backup Pipeline Initiated ===");
  Serial.println(" 🟡 WARNING: Voltage drop detected! Preparing data evacuation...");

  // Initialize both hardware SPI buses
  FLASH_SPI.begin();
  SD_SPI.begin();

  // Mount SPI Flash Peripheral
  if (!flash.begin()) {
    Serial.println(" 🔴 CRITICAL ERROR: SPI Flash not responding on PB12!");
    while (1);
  }

  // Mount SD Card Peripheral
  if (!sd.begin(SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(10), &SD_SPI))) {
    Serial.println(" 🔴 CRITICAL ERROR: SD Card not responding on PC9!");
    while (1);
  }
  Serial.println(" 🟢 Hardware peripherals successfully linked on Dual SPI buses.");

  // ====================================================================
  // TB PREPARATION (DO NOT MODIFY)
  // ====================================================================
  sd.remove(textFilename); 
  // ====================================================================

  // ---------------------------------------------------------
  // TODO: Emergency Backup Logic Workspace
  // ---------------------------------------------------------
  uint8_t exportedBootCount = 0;

  // TODO 1: Read the last known boot count from Flash address 0x0000 
  // and store the result inside the 'exportedBootCount' variable.
  // Hint: Use flash.readBuffer(address, pointer_to_variable, size);
  // [Add your code here]


  Serial.print("Retrieved Flight Recorder Status. Last Boot Count: ");
  Serial.println(exportedBootCount);

  // TODO 2: Create/Overwrite a clean text file on the SD Card named "safe_boot.txt".
  // We want to open it in Write-Only, Create if missing, and Wipe older emergency logs if any.
  // Hint: Use backupFile.open(textFilename, O_WRONLY | O_CREAT | O_TRUNC);
  // [Add your code here]


  // TODO 3: Write the verification string along with the extracted count to the file.
  // The written string inside the file MUST look exactly like: "Last known boot count: 94"
  // Hint: Use backupFile.print("...") for the text, and backupFile.println(exportedBootCount);
  // [Add your code here]


  // TODO 4: Force flush data to physical flash storage instantly by CLOSING the file!
  // [Add your code here]


  Serial.println("Emergency backup committed to Telemetry Vault.");
  Serial.println("OBC entering Deep Sleep Mode now...");

  // ---------------------------------------------------------
  // Run Built-In Self-Test (BIST)
  // ---------------------------------------------------------
  runBackupMissionTestbench(textFilename, exportedBootCount);
}

void loop() {
  // Satellite system remains in power-saving sleep loop
}