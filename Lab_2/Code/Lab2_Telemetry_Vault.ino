/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 2.2: The Telemetry Vault (Template with Testbench)
 * Objective: Initialize SD Card via SPI, create/append a CSV flight log.
 * Concept: Satellites must store telemetry in CSV format to be "Store-and-Forward" ready.
 */

#include <SPI.h>
#include "SdFat_Adafruit_Fork.h"

// ====================================================================
// HARDWARE PIN DEFINITIONS (Pre-configured for FlatSat OBC)
// Do not modify these pins!
// ====================================================================
#define SD_CS   PC9  
#define SD_SCK  PC10  
#define SD_MISO PC11  
#define SD_MOSI PC12  

SPIClass SD_SPI(SD_MOSI, SD_MISO, SD_SCK);
SdFat sd;
File flightFile;

const char* filename = "flightlog.csv";

// Mock telemetry data simulating sensors reading
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

  SD_SPI.begin();

  // Mount the SD Card
  if (!sd.begin(SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(10), &SD_SPI))) {
    Serial.println("❌ CRITICAL ERROR: SD Card mount failed! Check insertion.");
    while (1); 
  }
  Serial.println("✅ SD Card filesystem mounted successfully.");

  // =========================================================
  // 🐛 BIST PREPARATION (DO NOT MODIFY)
  // We intentionally delete the file if it's the first run 
  // to ensure you write the CSV header correctly!
  // =========================================================
  // sd.remove(filename); // Uncomment this line if you want to force a clean slate for testing.
  // =========================================================

  // ---------------------------------------------------------
  // TODO: Telemetry Logging Logic
  // ---------------------------------------------------------

  // TODO 1: Open 'flightlog.csv' in Append Mode.
  // We want to Create the file if it doesn't exist, Read/Write, and Append at the End.
  // Hint: Use flightFile.open(filename, O_RDWR | O_CREAT | O_AT_END);
  // [Add your code here]
  

  // TODO 2: Structural Check - If the file is brand new (size is 0), write the CSV header.
  // The header MUST be exactly: Boot_Count,Timestamp,Battery_V,OBC_Temp
  // Hint: Use flightFile.println("...");
  // [Add your code here]
  

  // TODO 3: Construct and write the telemetry row entry (CSV format).
  // Write the variables: currentBootCount, mockTimestamp, mockBatteryV, mockObcTemp
  // Remember to use flightFile.print(",") to separate fields, and use println() for the very last variable!
  // [Add your code here]
  

  // TODO 4: Critical Operation - CLOSE the file buffer!
  // If you don't close the file, the data will be lost when power is cut.
  // [Add your code here]
  

  // ---------------------------------------------------------
  // Run Built-In Self-Test (BIST)
  // ---------------------------------------------------------
  runTelemetryVaultTestbench();
}

void loop() {
  // Telemetry log runs once per boot
}

// ====================================================================
// 🛑 TESTBENCH SECTION - DO NOT MODIFY THIS CODE 🛑
// This function verifies file system structure and append mode logic.
// ====================================================================
void runTelemetryVaultTestbench() {
  Serial.println("\n--- Running BIST (Telemetry Vault Test) ---");
  bool isPassed = true;

  File testFile;
  
  // Test 1: File Existence (Verifies TODO 1 & TODO 4)
  if (!testFile.open(filename, O_RDONLY)) {
    Serial.println("❌ [FAIL] File Check: flightlog.csv not found.");
    Serial.println("   * Hint: Did you complete TODO 1 and close the file in TODO 4?");
    isPassed = false;
  } else {
    Serial.println("✅ [PASS] File Check: flightlog.csv detected on partition.");
    
    // Test 2: Header Syntax (Verifies TODO 2)
    String headerLine = testFile.readStringUntil('\n');
    headerLine.trim();
    if (headerLine != "Boot_Count,Timestamp,Battery_V,OBC_Temp") {
      Serial.println("❌ [FAIL] Structural Check: CSV headers mismatch or missing.");
      Serial.println("   -> Found:    \"" + headerLine + "\"");
      Serial.println("   -> Expected: \"Boot_Count,Timestamp,Battery_V,OBC_Temp\"");
      Serial.println("   * Hint: Did you write the exact header in TODO 2?");
      isPassed = false;
    } else {
      Serial.println("✅ [PASS] Structural Check: CSV database schema layout is correct.");
    }
    
    // Test 3: Data Appending (Verifies TODO 3)
    int dataRowsCount = 0;
    while (testFile.available()) {
      String dumpRow = testFile.readStringUntil('\n');
      if (dumpRow.length() > 5 && dumpRow.indexOf(',') > 0) { // Check for length and commas
        dataRowsCount++;
      }
    }
    testFile.close();
    
    if (dataRowsCount > 0) {
      Serial.println("✅ [PASS] Data Check: Valid telemetry records found (" + String(dataRowsCount) + " rows).");
    } else {
      Serial.println("❌ [FAIL] Data Check: No valid comma-separated records found.");
      Serial.println("   * Hint: Did you print the variables with commas in TODO 3?");
      isPassed = false;
    }
  }
  
  if (isPassed) {
    Serial.println("🌟 [SYSTEM READY] Telemetry Vault is stable. Data pipeline secure.");
  } else {
    Serial.println("⚠️ [SYSTEM WARNING] Telemetry storage fault detected. Review write logic.");
  }
  Serial.println("-------------------------------------------\n");
}