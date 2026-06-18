/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 6: Resilient Downlink & System Fault Tolerance
 * Module: On-Board Computer (OBC) - Skeleton Code
 * Architecture: OBC manages NVM state, reads SD Card, and delegates TX to COMMS.
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
HardwareSerial CommsUART(PA1, PA0);

// ====================================================================
// MISSION PARAMETERS
// ====================================================================
#define CHUNK_SIZE 48          // Optimized payload chunk size (bytes)
#define ACK_TIMEOUT_MS 2000    // Handshake wait threshold (milliseconds)
#define WDT_TIMEOUT_US 4000000 // Watchdog hardware trigger (4 seconds)

const char *IMAGE_FILE = "photo.jpg";
const char *STATE_FILE = "state.txt";

uint32_t currentChunkIndex = 0;
bool isTransferComplete = false;
bool isRecovered = false;

// ====================================================================
// STATE RECOVERY FUNCTIONS (NVM MANAGEMENT)
// ====================================================================
void loadTransferState()
{
    // ------------------------------------------------------------------
    // TODO 1: OBC STATE RECOVERY (Post-Reboot)
    // Hint: How does the OBC remember where it left off after a crash?
    // 1. Check if 'STATE_FILE' exists on the SD Card.
    // 2. Open it in READ ONLY mode (O_RDONLY).
    // 3. Read the string, convert it to an integer, and save it to 'currentChunkIndex'.
    // 4. Set 'isRecovered = true;' to mark successful recovery.
    // 5. Close the file.
    // ------------------------------------------------------------------

    /* --- YOUR CODE HERE --- */

    // if (sd.exists(STATE_FILE)) {
    //    Serial.print("[RECOVERY] Resuming transfer from Chunk #");
    //    Serial.println(currentChunkIndex);
    // } else {
    //    Serial.println("[START] New transfer mission. Starting from Chunk #0");
    // }
}

void saveTransferState(uint32_t chunkIdx)
{
    // ------------------------------------------------------------------
    // TODO 2: OBC CHECKPOINT SAVING
    // Hint: Save the current progress to the SD Card to survive a power loss.
    // 1. Open 'STATE_FILE' in WRITE mode (O_WRONLY | O_CREAT | O_TRUNC).
    // 2. Print 'chunkIdx' into the file.
    // 3. Close the file immediately to flush the buffer to disk.
    // ------------------------------------------------------------------

    /* --- YOUR CODE HERE --- */
}

// ====================================================================
// SETUP FUNCTION
// ====================================================================
void setup()
{
    Serial.setRx(PD9);
    Serial.setTx(PD8);
    Serial.begin(115200);
    CommsUART.begin(115200);

    delay(2000);
    Serial.println("\n=== FlatSat OBC: Resilient Downlink Manager ===");

    Serial.print("[SYSTEM] Initializing SD Card on SPI3... ");
    SD_SPI.begin();
    if (!sd.begin(SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(10), &SD_SPI)))
    {
        Serial.println("FAILED! Please check SD Card connection.");
        while (true)
            ;
    }
    Serial.println("OK!");

    // Load previous state (Fault Recovery Mechanism)
    loadTransferState();

    // ------------------------------------------------------------------
    // TODO 3: WATCHDOG TIMER INITIALIZATION
    // Hint: Arm the Hardware Watchdog Timer using 'WDT_TIMEOUT_US'.
    // If the system freezes, WDT will force a hardware reboot.
    // ------------------------------------------------------------------

    /* --- YOUR CODE HERE --- */
    // IWatchdog.begin( ??? );

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

    // ⚠️ แจกตัวแปรให้ก่อนเพื่อไม่ให้เกิด Error ในระบบที่บังคับให้ไว้ด้านล่าง
    File imgFile;

    // ------------------------------------------------------------------
    // TODO 4: OPEN IMAGE FILE
    // Hint: Use imgFile.open() to open 'IMAGE_FILE' in READ ONLY mode (O_RDONLY).
    // If it fails, print an error and service the Watchdog Timer.
    // ------------------------------------------------------------------

    /* --- YOUR CODE HERE --- */

    // if (!imgFile.open( ??? , ??? ))
    // {
    //     Serial.println("[ERROR] Cannot open image file. Did you save photo.jpg?");
    //     // Kick the dog here too!
    //     // ???
    //     delay(1000);
    //     return;
    // }

    // File System Check: Prevent seek overflow and detect mission end boundary
    if ((currentChunkIndex * CHUNK_SIZE) >= imgFile.fileSize())
    {
        imgFile.close();
        uint8_t eotPacket[3] = {0xFF, 0xFF, 0x00}; // End Of Transmission

        Serial.print("[TX] Sending EOT (End of Transmission) flag to COMMS... ");
        while (CommsUART.available())
        {
            CommsUART.read();
        }
        CommsUART.write(eotPacket, 3);

        unsigned long startTime = millis();
        bool gotAck = false;
        while (millis() - startTime < ACK_TIMEOUT_MS)
        {
            if (CommsUART.available() && CommsUART.readStringUntil('\n').indexOf("ACK") >= 0)
            {
                gotAck = true;
                break;
            }
        }

        if (gotAck)
        {
            Serial.println("OK! (ACK Received)\n[SUCCESS] Entire image transmitted to Earth successfully!");
            isTransferComplete = true;
            sd.remove(STATE_FILE); // Purge state file upon complete download
        }
        else
        {
            Serial.println("TIMEOUT! Will retry EOT next loop.");
        }
        IWatchdog.reload();
        delay(1000);
        return;
    }

    // ------------------------------------------------------------------
    // TODO 5: FILE POINTER MANAGEMENT (SEEK)
    // Hint: Move the file pointer to the correct chunk offset.
    // Offset = currentChunkIndex multiplied by CHUNK_SIZE.
    // ------------------------------------------------------------------

    /* --- YOUR CODE HERE --- */
    // imgFile.seek( ??? );

    uint8_t buffer[CHUNK_SIZE + 4];

    // Packet Header Construction
    buffer[0] = (currentChunkIndex >> 8) & 0xFF;
    buffer[1] = currentChunkIndex & 0xFF;

    // ⚠️ แจกตัวแปรให้ก่อนเพื่อใช้แนบ Header ในบรรทัดถัดไป
    int bytesRead = 0;

    // ------------------------------------------------------------------
    // TODO 6: READ PAYLOAD FROM DISK
    // Hint: Read CHUNK_SIZE bytes from the SD Card directly into the buffer,
    // starting at index 3 (saving index 0-2 for the header).
    // 1. Assign the return value of read() to 'bytesRead'.
    // 2. Close the file immediately to free up OBC memory.
    // ------------------------------------------------------------------

    /* --- YOUR CODE HERE --- */
    // bytesRead = imgFile.read( ??? , ??? );
    // imgFile.close();

    // Inject actual data length into Header
    buffer[2] = bytesRead & 0xFF;
    size_t packetSize = bytesRead + 3;

    // ==================================================================
    // PROVIDED: SIMULATED SINGLE EVENT UPSET (RADIATION FAULT)
    // Note to Student: DO NOT modify this block. This simulates a radiation
    // strike that freezes the CPU at Chunk #10.
    // Your WDT (TODO 3 & 7) and Recovery (TODO 1 & 2) must handle this!
    // ==================================================================
    if (currentChunkIndex == 10 && !isRecovered)
    {
        Serial.println("\n[WARNING] RADIATION SPIKE DETECTED! SEU OCCURRED!");
        Serial.println("System enters infinite loop. Waiting for Watchdog to intervene...");
        saveTransferState(currentChunkIndex); // Ensure state is saved before crash
        while (true)
        {
            // SYSTEM FROZEN! If WDT is not armed, the satellite is dead.
        }
    }

    // ------------------------------------------------------------------
    // Data Transmission Subsystem (OBC -> COMMS)
    // ------------------------------------------------------------------
    Serial.print("[TX] Sending Chunk #");
    Serial.print(currentChunkIndex);
    Serial.print(" to COMMS... ");
    while (CommsUART.available())
    {
        CommsUART.read();
    }
    CommsUART.write(buffer, packetSize);

    // Handshake Mechanism (Wait for ACK from COMMS)
    unsigned long startTime = millis();
    bool gotAck = false;
    while (millis() - startTime < ACK_TIMEOUT_MS)
    {
        if (CommsUART.available() && CommsUART.readStringUntil('\n').indexOf("ACK") >= 0)
        {
            gotAck = true;
            break;
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

    // ------------------------------------------------------------------
    // TODO 7: KICK THE DOG (WATCHDOG RELOAD)
    // Hint: Service the Watchdog Timer to prevent it from resetting the
    // system during normal operation.
    // ------------------------------------------------------------------

    /* --- YOUR CODE HERE --- */
    // IWatchdog.reload();
}