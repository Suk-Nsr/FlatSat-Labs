/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 6: Resilient Downlink & Radio Relay Subsystem
 * Module: Communication Module (COMMS) - Skeleton Code
 * Architecture: Receives dynamic data via UART, computes Checksum, encodes KISS, and transmits over RF.
 */

#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

// ====================================================================
// HARDWARE PIN DEFINITIONS (COMMS Subsystem - STM32F411RE)
// ====================================================================
#define RADIO_SCK PA5
#define RADIO_MISO PA6
#define RADIO_MOSI PA7
#define RADIO_NSS PB6
#define RADIO_DIO0 PA10
#define RADIO_RESET PC7
#define RADIO_DIO1 -1

SX1278 radio = new Module(RADIO_NSS, RADIO_DIO0, RADIO_RESET, RADIO_DIO1, SPI);

// --- UART TO OBC MODULE ---
HardwareSerial ObcUART(PA12, PA11);

// ====================================================================
// KISS PROTOCOL DEFINITIONS & ENCODER
// ====================================================================
#define FEND 0xC0
#define FESC 0xDB
#define TFEND 0xDC
#define TFESC 0xDD

size_t encodeKISS(const uint8_t *payload, size_t payloadSize, uint8_t *outBuffer)
{
    // ------------------------------------------------------------------
    // TODO 1: KISS PROTOCOL ENCODER
    // Hint: Implement the Byte Stuffing logic from previous labs.
    // Replace FEND with [FESC, TFEND] and FESC with [FESC, TFESC].
    // Do not forget to wrap the entire packet with FEND at the start and end.
    // ------------------------------------------------------------------

    /* --- YOUR CODE HERE --- */
}

// ====================================================================
// SETUP FUNCTION
// ====================================================================
void setup()
{
    Serial.begin(115200);  // Debug to PC USB
    ObcUART.begin(115200); // Inter-board connection
    delay(2000);
    Serial.println("\n=== FlatSat COMMS: Radio Relay Active ===");

    SPI.setMISO(RADIO_MISO);
    SPI.setMOSI(RADIO_MOSI);
    SPI.setSCLK(RADIO_SCK);
    SPI.begin();

    Serial.print("[SYSTEM] Initializing RF Module... ");
    if (radio.beginFSK() == RADIOLIB_ERR_NONE)
    {
        // ------------------------------------------------------------------
        // TODO 2: RF MODULE CONFIGURATION
        // Hint: Configure the radio parameters. Coordinate with your instructor
        // to assign a specific Frequency (around 433.0 MHz) to avoid collision
        // with other groups. Set BitRate to 9.6 kbps and OutputPower to 2 dBm.
        // ------------------------------------------------------------------

        /* --- YOUR CODE HERE --- */

        Serial.println("OK!");
    }
    else
    {
        Serial.println("FAILED!");
        while (true)
            ;
    }

    Serial.println("[SYSTEM] Flushing phantom UART data...");
    delay(500);

    // ------------------------------------------------------------------
    // TODO 3: PURGE PHANTOM DATA (STARTUP)
    // Hint: Clear any junk data in the UART buffer caused by startup transients.
    // Use a while loop with ObcUART.available() and ObcUART.read().
    // ------------------------------------------------------------------

    /* --- YOUR CODE HERE --- */

    Serial.println("[SYSTEM] System Ready.");
}

// ====================================================================
// MAIN LOOP FUNCTION
// ====================================================================
void loop()
{
    // ------------------------------------------------------------------
    // TODO 4: CHECK INCOMING UART DATA
    // Hint: Check if there are at least 3 bytes (Packet Header) available
    // in the ObcUART buffer before reading.
    // ------------------------------------------------------------------
    if (/* --- YOUR CODE HERE --- */)
    {
        uint8_t header[3];
        ObcUART.readBytes(header, 3);

        // ------------------------------------------------------------------
        // TODO 5: DECODE PACKET HEADER
        // Hint: Combine header[0] (High) and header[1] (Low) into a 16-bit chunkId.
        // Assign header[2] to payloadLen.
        // ------------------------------------------------------------------

        /* --- YOUR CODE HERE --- */
        // uint16_t chunkId = ???
        // uint8_t payloadLen = ???

        uint8_t rawPayload[100];
        rawPayload[0] = header[0];
        rawPayload[1] = header[1];
        rawPayload[2] = header[2];

        // 2. Read the remaining expected image block bytes
        ObcUART.setTimeout(250);
        size_t rxCount = ObcUART.readBytes(&rawPayload[3], payloadLen);

        if (rxCount == payloadLen)
        {
            size_t totalRawSize = 3 + payloadLen;

            // ------------------------------------------------------------------
            // TODO 6: COMPUTE ERROR DETECTION CHECKSUM (XOR-8)
            // Hint: Calculate the XOR-8 checksum of the entire 'rawPayload' array.
            // Loop from index 0 to 'totalRawSize'.
            // ------------------------------------------------------------------
            uint8_t checksum = 0;

            /* --- YOUR CODE HERE --- */

            rawPayload[totalRawSize] = checksum; // Append Checksum byte

            // ------------------------------------------------------------------
            // TODO 7: ENCAPSULATE WITH KISS PROTOCOL
            // Hint: Call your 'encodeKISS' function to wrap 'rawPayload' into 'txKISSBuffer'.
            // Pass the exact size of the payload (including checksum).
            // ------------------------------------------------------------------
            uint8_t txKISSBuffer[150];
            size_t kissPacketSize = 0;

            /* --- YOUR CODE HERE --- */
            // kissPacketSize = encodeKISS( ??? , ??? , ??? );

            Serial.print("[RELAY] Encoded Chunk #");
            Serial.print(chunkId);
            Serial.print(" (Len: ");
            Serial.print(payloadLen);
            Serial.print(") into KISS (");
            Serial.print(kissPacketSize);
            Serial.print(" bytes). Transmitting to Earth... ");

            // ------------------------------------------------------------------
            // TODO 8: TRANSMIT RF PACKET
            // Hint: Use radio.transmit() to send the 'txKISSBuffer' over the air.
            // Check if the transmission was successful.
            // ------------------------------------------------------------------

            /* --- YOUR CODE HERE --- */
            // int txState = radio.transmit( ??? , ??? );
            // if (txState != RADIOLIB_ERR_NONE)
            // {
            //     Serial.print("RF Error: ");
            //     Serial.print(txState);
            // }

            // 6. Standby and listen for the Ground Station RF Acknowledge (Stop-and-Wait ARQ)
            String rfResponse;
            int rxState = radio.receive(rfResponse, 1000); // 1-second downlink window

            // ------------------------------------------------------------------
            // TODO 9: VERIFY ACKNOWLEDGEMENT
            // Hint: Check if the received string ('rfResponse') contains the word "ACK".
            // Use the .indexOf() function of the String class.
            // ------------------------------------------------------------------
            if (rxState == RADIOLIB_ERR_NONE && /* --- YOUR CODE HERE --- */)
            {
                Serial.println(" -> Got RF ACK from Ground Station!");
                ObcUART.println("ACK"); // Pass handshake affirmation back to OBC
            }
            else
            {
                Serial.println(" -> RF Timeout / Drop. (Ignoring OBC, forcing retransmit)");
            }

            // ------------------------------------------------------------------
            // TODO 10: EMI NOISE MITIGATION (FLUSH UART)
            // Hint: High-power RF transmission might induce phantom noise on the UART line.
            // Flush the ObcUART buffer here just like you did in setup().
            // ------------------------------------------------------------------

            /* --- YOUR CODE HERE --- */
        }
        else
        {
            Serial.println("[ERROR] UART Payload fragmented. Flushing buffer.");

            // ------------------------------------------------------------------
            // TODO 11: FLUSH FRAGMENTED DATA
            // Hint: Flush the remaining broken data to prepare for the next clean packet.
            // ------------------------------------------------------------------

            /* --- YOUR CODE HERE --- */
        }
    }
}