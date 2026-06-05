/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 2.2: The Telemetry Vault (Solution Key)
 * Objective: Initialize SD Card via SPI3, create/append a CSV flight log.
 */

#include <SPI.h>
#include "SdFat_Adafruit_Fork.h"

// ====================================================================
// ⚠️ HARDWARE PIN DEFINITIONS FOR FLATSAT OBC ⚠️
// Based on Payload subsystem documentation (SPI3 pins)
// ====================================================================
#define SD_CS   PC9   // Chip Select for SD Card
#define SD_SCK  PC10  // Serial Clock (SPI3)
#define SD_MISO PC11  // Master In Slave Out (SPI3)
#define SD_MOSI PC12  // Master Out Slave In (SPI3)

// Create a dedicated hardware SPI object (SPI3)
SPIClass SD_SPI(SD_MOSI, SD_MISO, SD_SCK);

// Initialize SdFat instance
SdFat sd;
File flightFile;

const char* filename = "flightlog.csv";

// Mock telemetry data
uint8_t currentBootCount = 85;
String mockTimestamp    = "12:34:56";
float mockBatteryV      = 4.12;
float mockObcTemp       = 28.50;

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  
  while(!Serial){;}

  Serial.println("\n=== FlatSat Telemetry Vault Booting ===");

  // Initialize the dedicated SPI3 bus
  SD_SPI.begin();

  // Mount SD Card using the specific CS pin and SPI3 bus
  // Using DEDICATED_SPI since this bus is only for the SD Card
  if (!sd.begin(SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(10), &SD_SPI))) {
    Serial.println("❌ CRITICAL ERROR: SD Card mount failed!");
    Serial.print("   SdFat Error Details: ");
    sd.printSdError(&Serial);
    while (1); // Halt system
  }
  Serial.println("✅ SD Card filesystem mounted successfully.");

  // ---------------------------------------------------------
  // TODO: Telemetry Logging Logic (FILLED SOLUTION)
  // ---------------------------------------------------------

  // 1. Open file in Append Mode
  if (!flightFile.open(filename, O_RDWR | O_CREAT | O_AT_END)) {
    Serial.println("❌ ERROR: Failed to open flightlog.csv!");
    while (1);
  }

  // 2. Write header if new file
  if (flightFile.size() == 0) {
    Serial.println("📂 New log file detected. Injecting CSV headers...");
    flightFile.println("Boot_Count,Timestamp,Battery_V,OBC_Temp");
  }

  // 3. Append data
  flightFile.print(currentBootCount);
  flightFile.print(",");
  flightFile.print(mockTimestamp);
  flightFile.print(",");
  flightFile.print(mockBatteryV, 2);
  flightFile.print(",");
  flightFile.println(mockObcTemp, 2);

  // 4. Close the file!
  flightFile.close();
  Serial.println("💾 Telemetry frame committed to disk storage.");

  runTelemetryVaultTestbench();
}

void loop() {}

// ====================================================================
// 🛑 TESTBENCH SECTION - DO NOT MODIFY THIS CODE 🛑
// ====================================================================
void runTelemetryVaultTestbench() {
  Serial.println("\n--- Running BIST (Telemetry Vault Test) ---");
  bool isPassed = true;

  File testFile;
  if (!testFile.open(filename, O_RDONLY)) {
    Serial.println("❌ [FAIL] File Check: flightlog.csv not found.");
    isPassed = false;
  } else {
    Serial.println("✅ [PASS] File Check: flightlog.csv detected.");
    
    // Check Header
    String headerLine = testFile.readStringUntil('\n');
    headerLine.trim();
    if (headerLine != "Boot_Count,Timestamp,Battery_V,OBC_Temp") {
      Serial.println("❌ [FAIL] Structural Check: CSV headers mismatch.");
      isPassed = false;
    } else {
      Serial.println("✅ [PASS] Structural Check: CSV headers correct.");
    }
    
    // Count entries
    int dataRowsCount = 0;
    while (testFile.available()) {
      if (testFile.readStringUntil('\n').length() > 2) dataRowsCount++;
    }
    testFile.close();
    
    if (dataRowsCount > 0) {
      Serial.println("✅ [PASS] Data Check: Records found: " + String(dataRowsCount));
    } else {
      Serial.println("❌ [FAIL] Data Check: No records found.");
      isPassed = false;
    }
  }
  
  if (isPassed) Serial.println("🌟 [SYSTEM READY] Telemetry Vault is stable.");
  else Serial.println("⚠️ [SYSTEM WARNING] Telemetry storage fault.");
  Serial.println("-------------------------------------------\n");
}