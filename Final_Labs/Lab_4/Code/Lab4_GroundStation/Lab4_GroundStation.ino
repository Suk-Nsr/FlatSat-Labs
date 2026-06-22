/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 4: Resilient Downlink (Ground Station Receiver)
 * Module: Ground Station - Skeleton Code
 * Architecture: Interrupt-based RF Receive -> KISS Decode -> Verify Checksum -> Transmit ACK -> Serial to PC
 */

#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

// ====================================================================
// HARDWARE PIN DEFINITIONS (Ground Station Module - STM32F103RC)
// ====================================================================
#define RADIO_SCK PA5
#define RADIO_MISO PA6
#define RADIO_MOSI PA7
#define RADIO_NSS PB6
#define RADIO_DIO0 PA10
#define RADIO_RESET PC7
#define RADIO_DIO1 -1

SX1278 radio = new Module(RADIO_NSS, RADIO_DIO0, RADIO_RESET, RADIO_DIO1, SPI);

// ====================================================================
// INTERRUPT FLAG & ISR (Interrupt Service Routine)
// ====================================================================
volatile bool receivedFlag = false;

void setFlag(void)
{
    receivedFlag = true;
}

// ====================================================================
// KISS PROTOCOL DEFINITIONS & DECODER
// ====================================================================
#define FEND 0xC0
#define FESC 0xDB
#define TFEND 0xDC
#define TFESC 0xDD

size_t decodeKISS(const uint8_t *inBuffer, size_t inSize, uint8_t *outBuffer)
{
    // ------------------------------------------------------------------
    // TODO 1: KISS PROTOCOL DECODER (From Previous Labs)
    // Hint: Copy your KISS decoding logic from the previous lab.
    // Remember to handle the FEND boundaries and the escape sequences
    // (FESC followed by TFEND or TFESC).
    // ------------------------------------------------------------------

    /* --- YOUR CODE HERE --- */

    return 0; // Update this return value with the actual decoded size
}

bool verifyChecksum(const uint8_t *payload, size_t length)
{
    // ------------------------------------------------------------------
    // TODO 2: CHECKSUM VERIFICATION (From Previous Labs)
    // Hint: Copy your XOR-8 checksum verification logic here.
    // Ensure the calculated checksum of all bytes matches the final byte
    // appended at the end of the payload array.
    // ------------------------------------------------------------------

    /* --- YOUR CODE HERE --- */

    return false; // Replace with your boolean evaluation
}

// ====================================================================
// SETUP FUNCTION
// ====================================================================
void setup()
{
    Serial.setTx(PC10);
    Serial.setRx(PC11);
    Serial.begin(115200);
    delay(2000);

    Serial.println("\n=== FlatSat Ground Station: Image Assembler Ready ===");

    SPI.setMISO(RADIO_MISO);
    SPI.setMOSI(RADIO_MOSI);
    SPI.setSCLK(RADIO_SCK);
    SPI.begin();

    if (radio.beginFSK() == RADIOLIB_ERR_NONE)
    {
        // ------------------------------------------------------------------
        // TODO 3: RF MODULE CONFIGURATION
        // Hint: Configure the radio parameters to match your COMMS module.
        // Coordinate with your instructor to set the correct Frequency.
        // Set BitRate to 9.6 kbps and OutputPower to 2 dBm.
        // ------------------------------------------------------------------

        /* --- YOUR CODE HERE --- */

        // Bind hardware transceiver event to ISR flag
        radio.setDio0Action(setFlag, RISING);

        // Trigger baseline unblocking background reception
        int state = radio.startReceive();
        if (state == RADIOLIB_ERR_NONE)
        {
            Serial.println("[SYSTEM] RF Interrupt enabled. Listening to the sky...");
        }
        else
        {
            Serial.print("[ERROR] Failed to start receive. Code: ");
            Serial.println(state);
        }
    }
    else
    {
        Serial.println("[ERROR] RF Module failed!");
        while (true)
            ;
    }
}

// ====================================================================
// MAIN LOOP FUNCTION
// ====================================================================
void loop()
{
    if (receivedFlag)
    {
        receivedFlag = false; // Reset background execution loop flag

        uint8_t rxBuffer[256];
        size_t rxSize = radio.getPacketLength();

        // ------------------------------------------------------------------
        // TODO 4: READ DATA FROM RF BUFFER (Interrupt Mode)
        // Hint: In interrupt mode, the data is already captured in the chip.
        // DO NOT use radio.receive(), use the function that simply 'reads'
        // the data into your rxBuffer.
        // ------------------------------------------------------------------

        int state = RADIOLIB_ERR_NONE; // Placeholder

        /* --- YOUR CODE HERE --- */
        // state = radio.???( ??? , ??? );

        if (state == RADIOLIB_ERR_NONE)
        {
            uint8_t payload[256];
            size_t payloadSize = decodeKISS(rxBuffer, rxSize, payload);

            if (payloadSize > 0)
            {
                if (verifyChecksum(payload, payloadSize))
                {
                    // ------------------------------------------------------------------
                    // TODO 5: DATA PIPELINE FORMATTING (SERIAL TO PYTHON)
                    // Hint: To send binary data over a text-based Serial monitor, we convert
                    // it to Hexadecimal format. Our Python script specifically looks for
                    // the token "CHUNK_DATA:" followed by the HEX string.
                    //
                    // 1. Print the keyword "CHUNK_DATA:" (without a newline).
                    // 2. Loop through the 'payload' array (EXCLUDING the checksum byte).
                    // 3. For each byte, if it is less than 16 (0x10), print a leading "0".
                    // 4. Print the byte in HEX format.
                    // 5. Print a newline character at the end of the loop.
                    // ------------------------------------------------------------------

                    /* --- YOUR CODE HERE --- */

                    // ------------------------------------------------------------------
                    // TODO 6: TRANSMIT ACKNOWLEDGE
                    // Hint: Dispatch a stop-and-wait acknowledgement ("ACK") back to space.
                    // Add a very small delay (e.g., 50ms) before transmitting to allow
                    // the satellite's transceiver to switch to receive mode.
                    // ------------------------------------------------------------------

                    /* --- YOUR CODE HERE --- */
                }
                else
                {
                    Serial.println("[WARNING] Checksum Mismatch! Packet Dropped.");
                }
            }
            else
            {
                Serial.println("[WARNING] KISS Decode failed! Incomplete frame.");
            }
        }
        else if (state == RADIOLIB_ERR_CRC_MISMATCH)
        {
            Serial.println("[WARNING] RF CRC Mismatch! Bad signal received.");
        }

        // ------------------------------------------------------------------
        // TODO 7: REACTIVATE LISTENING MODE & CLEAR GHOST ARTIFACTS
        // Hint:
        // 1. You MUST command the radio to go back to listening mode using startReceive().
        // 2. Transmitting the ACK in TODO 6 will trigger the DIO0 interrupt again (TxDone).
        //    You must set 'receivedFlag = false' immediately after startReceive() to
        //    clear this ghost interrupt.
        // ------------------------------------------------------------------

        /* --- YOUR CODE HERE --- */
    }
}