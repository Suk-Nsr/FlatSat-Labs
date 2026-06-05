/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 2.3: The Backup Mission (Template with Testbench)
 * Objective: Integrate Dual SPI Buses to transfer data from Flash to SD Card.
 * Scenario: OBC detects a power brownout, copies Boot Count from Flash,
 * and safely creates an emergency file on the SD Card before Deep Sleep.
 */

#include <SPI.h>
#include "SdFat_Adafruit_Fork.h"

// Define that we are using standard SPI (not QSPI) for Flash
#define EXTERNAL_FLASH_USE_SPI 1
#include <Adafruit_SPIFlash.h>

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
  Serial.println("⚠️ WARNING: Voltage drop detected! Preparing data evacuation...");

  // Initialize both hardware SPI buses
  FLASH_SPI.begin();
  SD_SPI.begin();

  // Mount SPI Flash (on SPI2)
  if (!flash.begin()) {
    Serial.println("❌ CRITICAL ERROR: SPI Flash not responding on PB12!");
    while (1);
  }

  // Mount SD Card (on SPI3 using Dedicated Mode)
  if (!sd.begin(SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(10), &SD_SPI))) {
    Serial.println("❌ CRITICAL ERROR: SD Card not responding on PC9!");
    while (1);
  }
  Serial.println("✅ Hardware peripherals successfully linked on Dual SPI buses.");

  // =========================================================
  // 🐛 BIST PREPARATION (DO NOT MODIFY)
  // Remove any old backup file to ensure you generate a new one
  // =========================================================
  sd.remove(textFilename); 
  // =========================================================

  // ---------------------------------------------------------
  // TODO: Emergency Backup Logic
  // ---------------------------------------------------------

  uint8_t exportedBootCount = 0;

  // TODO 1: Read the last known boot count from Flash address 0x0000 
  // and store it in the 'exportedBootCount' variable.
  // Hint: Use flash.readBuffer(address, pointer_to_variable, size);
  // [Add your code here]
  

  Serial.print("ℹ️ Retrieved Flight Recorder Status. Last Boot Count: ");
  Serial.println(exportedBootCount);

  // TODO 2: Create a clean file on the SD Card named "safe_boot.txt"
  // Hint: Use backupFile.open(filename, O_WRONLY | O_CREAT | O_TRUNC)
  // Check if opening failed and halt the system if it did.
  // [Add your code here]
  

  // TODO 3: Write the verification string along with the extracted count.
  // Example output: "Last known boot count: 85"
  // Hint: Use backupFile.print() for the string, and backupFile.println() for the number.
  // [Add your code here]
  

  // TODO 4: Force flush data to the physical storage instantly by closing the file!
  // [Add your code here]
  

  Serial.println("💾 Emergency backup committed to Telemetry Vault.");
  Serial.println("💤 OBC entering Deep Sleep Mode now...");

  // ---------------------------------------------------------
  // Run Built-In Self-Test (BIST)
  // ---------------------------------------------------------
  runBackupMissionTestbench(exportedBootCount);
}

void loop() {
  // Satellite system remains in power-saving sleep loop
}

// ====================================================================
// 🛑 TESTBENCH SECTION - DO NOT MODIFY THIS CODE 🛑
// This function verifies cross-peripheral data transportation.
// ====================================================================
void runBackupMissionTestbench(uint8_t originalCount) {
  Serial.println("\n--- Running BIST (Emergency Backup Test) ---");
  bool isPassed = true;

  File testFile;
  // Test 1: Verify if the backup file exists
  if (!testFile.open(textFilename, O_RDONLY)) {
    Serial.println("❌ [FAIL] File Check: safe_boot.txt not found on the SD card.");
    Serial.println("   * Hint: Did you create the file in TODO 2 and close it in TODO 4?");
    isPassed = false;
  } else {
    Serial.println("✅ [PASS] File Check: safe_boot.txt verified on disk.");

    // Test 2: Parse and verify content integrity
    String fileContent = testFile.readString();
    testFile.close();
    fileContent.trim(); // Clean trailing whitespaces

    Serial.println("ℹ️ Reading File Content: \"" + fileContent + "\"");

    // Reconstruct expected string based on the Flash variable
    String expectedContent = "Last known boot count: " + String(originalCount);

    if (fileContent != expectedContent || originalCount == 0) {
      Serial.println("❌ [FAIL] Content Check: Data corruption or missing data detected.");
      Serial.println("   -> Found:    \"" + fileContent + "\"");
      Serial.println("   -> Expected: \"" + expectedContent + "\"");
      Serial.println("   * Hint: Did you properly read from Flash (TODO 1) and write to SD (TODO 3)?");
      isPassed = false;
    } else {
      Serial.println("✅ [PASS] Content Check: Flash-to-SD pipeline data matches perfectly.");
    }
  }

  // Final Evaluation
  if (isPassed) {
    Serial.println("🌟 [MISSION ACCOMPLISHED] Data evacuation successful. System safe to power off.");
  } else {
    Serial.println("❌ [MISSION FAILED] Data lost in transmission. Check your code logic.");
  }
  Serial.println("-------------------------------------------\n");
}