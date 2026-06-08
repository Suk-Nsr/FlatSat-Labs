/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 2.2: The Telemetry Vault (Student Template)
 * Objective: Initialize SD Card via SPI, create/append a CSV flight log.
 * Concept: Satellites must store telemetry in CSV format to be "Store-and-Forward" ready.
 */

#include <SPI.h>
#include "SdFat_Adafruit_Fork.h"

#include "src/Lab2_TB_Telemetry_Vault.h"

// ====================================================================
// HARDWARE PIN DEFINITIONS (Pre-configured for FlatSat OBC)
// Do not modify these pins!
// ====================================================================
#define SD_CS PC9
#define SD_SCK PC10
#define SD_MISO PC11
#define SD_MOSI PC12

SPIClass SD_SPI(SD_MOSI, SD_MISO, SD_SCK);
SdFat sd;
File flightFile;
const char *filename = "flightlog.csv";

// Mock telemetry data simulating sensors reading
uint8_t currentBootCount = 85;
String mockTimestamp = "12:34:56";
float mockBatteryV = 4.12;
float mockObcTemp = 28.50;

void setup()
{
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  while (!Serial)
  {
    ;
  }

  Serial.println("\n=== FlatSat Telemetry Vault Booting ===");
  SD_SPI.begin();

  // Mount the MicroSD Card Filesystem
  if (!sd.begin(SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(10), &SD_SPI)))
  {
    Serial.println(" 🔴 CRITICAL ERROR: SD Card not responding! Check insertion.");
    while (1)
      ;
  }
  Serial.println(" 🟢 SD Card filesystem mounted successfully.");

  // =========================================================
  // TB PREPARATION (MODIFIABLE)
  // =========================================================
  sd.remove(filename); // Comment this line if you want continuous data slate for testing.
  // =========================================================

  // ---------------------------------------------------------
  // TODO: Telemetry Logging Logic Workspace
  // ---------------------------------------------------------

  // TODO 1: Open the file 'flightlog.csv' in Append Mode.
  // We want to Create the file if it doesn't exist, Read/Write, and Append at the End.
  // Hint: Use flightFile.open(filename, O_RDWR | O_CREAT | O_AT_END);
  // [Add your code here]

  // TODO 2: Structural Check - If the file is brand new (size is 0), write the CSV header.
  // The header string MUST be exactly: "Boot_Count,Timestamp,Battery_V,OBC_Temp"
  // Hint: Check file size using flightFile.size() and write using flightFile.println("...");
  // [Add your code here]

  // TODO 3: Construct and write the telemetry row entry (Comma-Separated Format).
  // Write variables: currentBootCount, mockTimestamp, mockBatteryV, mockObcTemp
  // Remember to use flightFile.print(",") to separate fields, and println() for the very last variable!
  // Hint: Keep float values at 2 decimal places using print(variable, 2);
  // [Add your code here]

  // TODO 4: Critical Safe Operation - CLOSE the file buffer!
  // If you don't close the file, data blocks inside the cache will be corrupted upon power cut.
  // [Add your code here]

  Serial.println("Telemetry frame processing completed.");

  // ---------------------------------------------------------
  // Run Testbench (TB)
  // ---------------------------------------------------------
  runTelemetryVaultTestbench(filename);
}

void loop()
{
  // Telemetry log runs once per boot cycle
}