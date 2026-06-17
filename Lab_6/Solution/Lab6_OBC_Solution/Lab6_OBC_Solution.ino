/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 6: Resilient Downlink & System Fault Tolerance
 * Module: On-Board Computer (OBC) - Solution Code
 * Architecture: OBC reads SD Card and sends data chunks to COMMS via UART.
 */

#include <Arduino.h>
#include <SPI.h>
#include "SdFat_Adafruit_Fork.h"
#include <IWatchdog.h>

// ====================================================================
// HARDWARE PIN DEFINITIONS
// ====================================================================

// --- SD CARD BUS (SPI3 - Dedicated) ---
#define SD_SCK PC10
#define SD_MISO PC11
#define SD_MOSI PC12
#define SD_CS PC9

SPIClass SD_SPI(SD_MOSI, SD_MISO, SD_SCK);
SdFat sd;

// --- UART TO COMMS MODULE ---
// OBC uses PA1 (RX) and PA0 (TX) to communicate with the COMMS board
HardwareSerial CommsUART(PA1, PA0);

// ====================================================================
// MISSION PARAMETERS
// ====================================================================
#define CHUNK_SIZE 48
#define ACK_TIMEOUT_MS 2000    // Wait up to 2 seconds for COMMS to reply
#define WDT_TIMEOUT_US 4000000 // Watchdog Timeout set to 4 seconds

const char *IMAGE_FILE = "photo.jpg";
const char *STATE_FILE = "state.txt";

uint32_t currentChunkIndex = 0;
bool isTransferComplete = false;
bool hasSeuOccurred = false;

// ====================================================================
// STATE RECOVERY FUNCTIONS
// ====================================================================
void loadTransferState()
{
  if (sd.exists(STATE_FILE))
  {
    File stateFile;
    if (stateFile.open(STATE_FILE, O_RDONLY))
    {
      String valStr = stateFile.readStringUntil('\n');
      currentChunkIndex = valStr.toInt();
      stateFile.close();
      hasSeuOccurred = true;
      Serial.print("[RECOVERY] Resuming transfer from Chunk #");
      Serial.println(currentChunkIndex);
    }
  }
  else
  {
    Serial.println("[START] New transfer mission. Starting from Chunk #0");
  }
}

void saveTransferState(uint32_t chunkIdx)
{
  File stateFile;
  if (stateFile.open(STATE_FILE, O_WRONLY | O_CREAT | O_TRUNC))
  {
    stateFile.println(chunkIdx);
    stateFile.close();
  }
}

// ====================================================================
// SETUP FUNCTION
// ====================================================================
void setup()
{
  // Initialize Debug Serial to PC
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);

  // Initialize UART connection to the COMMS subsystem
  CommsUART.begin(115200);

  delay(2000);
  Serial.println("\n=== FlatSat OBC: Resilient Downlink Manager ===");

  // 1. Initialize SD Card (on SPI3)
  Serial.print("[SYSTEM] Initializing SD Card on SPI3... ");
  SD_SPI.begin();
  if (!sd.begin(SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(10), &SD_SPI)))
  {
    Serial.println("FAILED!");
    Serial.println("-> Please check SD Card connection.");
    while (true)
      ;
  }
  Serial.println("OK!");

  // 2. Load previous state (Fault Recovery)
  loadTransferState();

  // 3. Initialize Hardware Watchdog Timer
  IWatchdog.begin(WDT_TIMEOUT_US);
  Serial.println("[SYSTEM] Watchdog Timer armed (4 Seconds).");
}

// ====================================================================
// MAIN LOOP FUNCTION
// ====================================================================
void loop()
{
  if (isTransferComplete)
  {
    IWatchdog.reload();
    delay(1000);
    return;
  }

  File imgFile;
  if (!imgFile.open(IMAGE_FILE, O_RDONLY))
  {
    Serial.println("[ERROR] Cannot open image file. Did you save photo.jpg?");
    IWatchdog.reload();
    delay(1000);
    return;
  }

  if ((currentChunkIndex * CHUNK_SIZE) >= imgFile.fileSize())
  {
    Serial.println("\n[SUCCESS] Entire image transmitted to COMMS successfully!");
    isTransferComplete = true;
    sd.remove(STATE_FILE);
    imgFile.close();
    return;
  }

  // Seek to the precise byte address of the current chunk
  imgFile.seek(currentChunkIndex * CHUNK_SIZE);

  uint8_t buffer[CHUNK_SIZE + 4];

  // Header: Embed the Chunk ID into the first 2 bytes
  buffer[0] = (currentChunkIndex >> 8) & 0xFF;
  buffer[1] = currentChunkIndex & 0xFF;

  int bytesRead = imgFile.read(&buffer[3], CHUNK_SIZE);
  imgFile.close();

  buffer[2] = bytesRead & 0xFF;
  size_t packetSize = bytesRead + 3;

  // ------------------------------------------------------------------
  // Simulated Single Event Upset (SEU) - Radiation Anomaly
  // ------------------------------------------------------------------

  if (currentChunkIndex == 10 && !hasSeuOccurred)
  {
    Serial.println("\n[WARNING] RADIATION SPIKE DETECTED! SEU OCCURRED!");
    Serial.println("System enters infinite loop. Waiting for Watchdog to intervene...");
    saveTransferState(currentChunkIndex);
    while (true)
    {
      // Infinite loop. WDT will trigger a hardware reboot after 4 seconds.
    }
  }

  // ------------------------------------------------------------------
  // Send data to COMMS via UART
  // ------------------------------------------------------------------
  Serial.print("[TX] Sending Chunk #");
  Serial.print(currentChunkIndex);
  Serial.print(" to COMMS via UART... ");

  // Clear any junk data in the incoming UART buffer
  while (CommsUART.available())
  {
    CommsUART.read();
  }

  // Send the actual bytes to the COMMS board
  CommsUART.write(buffer, packetSize);

  // ------------------------------------------------------------------
  // Wait for Handshake (ACK) from COMMS
  // ------------------------------------------------------------------
  unsigned long startTime = millis();
  bool gotAck = false;

  while (millis() - startTime < ACK_TIMEOUT_MS)
  {
    if (CommsUART.available())
    {
      String response = CommsUART.readStringUntil('\n');
      if (response.indexOf("ACK") >= 0)
      {
        gotAck = true;
        break;
      }
    }
  }

  if (gotAck)
  {
    Serial.println("OK! (ACK Received)");
    currentChunkIndex++;
    saveTransferState(currentChunkIndex);
  }
  else
  {
    Serial.println("TIMEOUT! (No ACK from COMMS). Will retransmit same chunk.");
  }

  IWatchdog.reload(); // Kick the dog to reset the 4-second timer
}