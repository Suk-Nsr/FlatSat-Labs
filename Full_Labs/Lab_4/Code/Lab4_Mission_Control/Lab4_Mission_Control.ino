/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 4.3: Mission Control - Ground Station (Student Template)
 * Objective: Initialize Ground Station RF, Decode KISS packets, and Verify Checksums.
 * Warning: This code must be uploaded directly to the GROUND STATION Board.
 */

#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

// Link the automated BIST system
#include "src/Lab4_TB_Mission_Control.h"

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

// Instantiate the SX1278 radio module object explicitly mapped to standard SPI bus
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
// KISS PROTOCOL DEFINITIONS
// ====================================================================
#define FEND 0xC0
#define FESC 0xDB
#define TFEND 0xDC
#define TFESC 0xDD

// --------------------------------------------------------------------
// TODO 1: Radio Hardware Initialization (STUDENT TEMPLATE)
// --------------------------------------------------------------------
int initRadioHardware(SX1278 &radioObj)
{
    // TODO 1.1: Initialize the radio using FSK modulation mode.
    int state = -1;

    if (state == RADIOLIB_ERR_NONE)
    {
        // TODO 1.2: Set Carrier Frequency to somewhere around 433.0 MHz (Must match Space Link!)

        // TODO 1.3: Set Data Rate to 9.6 kbps
    }
    return state;
}

// --------------------------------------------------------------------
// TODO 2: KISS Protocol Decoder (STUDENT TEMPLATE)
// --------------------------------------------------------------------
size_t decodeKISS(const uint8_t *inBuffer, size_t inSize, uint8_t *outBuffer)
{
    size_t outIndex = 0;
    bool inFrame = false;
    bool escapeNext = false;

    // TODO 2.1: Iterate through 'inBuffer' and implement the KISS un-stuffing logic.
    // - Identify FEND (0xC0) to mark the start and end of the frame.
    // - Ignore any garbage bytes outside the FEND markers.
    // - If you see FESC (0xDB), flag it and check the NEXT byte:
    //      - If next is TFEND (0xDC), write FEND (0xC0) to outBuffer.
    //      - If next is TFESC (0xDD), write FESC (0xDB) to outBuffer.
    // - Otherwise, write normal bytes directly to outBuffer.

    return outIndex; // Return the final decoded payload size
}

// --------------------------------------------------------------------
// TODO 3: Checksum Verification (STUDENT TEMPLATE)
// --------------------------------------------------------------------
bool verifyChecksum(const uint8_t *payload, size_t payloadSize)
{
    // TODO 3.1: Implement XOR Checksum validation.
    // - The LAST byte of the payload is the received checksum.
    // - Calculate the XOR sum of all bytes EXCEPT the last one.
    // - Return true if your calculated checksum matches the received checksum.

    return false;
}

void setup()
{
    Serial.setTx(PC10);
    Serial.setRx(PC11);
    Serial.begin(115200);
    delay(2000);

    Serial.println("\n=== FlatSat COMMS: Mission Control (Ground Station) ===");

    SPI.setMISO(RADIO_MISO);
    SPI.setMOSI(RADIO_MOSI);
    SPI.setSCLK(RADIO_SCK);
    SPI.begin();

    // Run BIST to test decoding and checksum logic
    runMissionControlTestbench(radio, initRadioHardware, decodeKISS, verifyChecksum);

    // Configure non-blocking interrupt parameters
    radio.setDio0Action(setFlag, RISING);
    int state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE)
    {
        Serial.println("\nGround Station is now listening for FlatSat Telemetry (Interrupt Mode)...");
    }
    else
    {
        Serial.print("\nFailed to start receiver, code: ");
        Serial.println(state);
    }
}

void loop()
{
    if (receivedFlag)
    {
        receivedFlag = false;

        uint8_t rxBuffer[256];
        size_t rxSize = radio.getPacketLength();
        int state = radio.readData(rxBuffer, rxSize);

        if (state == RADIOLIB_ERR_NONE)
        {
            uint8_t payload[256];
            size_t payloadSize = decodeKISS(rxBuffer, rxSize, payload);

            if (payloadSize > 0)
            {
                Serial.print("\nSignal Caught! RSSI: ");
                Serial.print(radio.getRSSI());
                Serial.println(" dBm");

                if (verifyChecksum(payload, payloadSize))
                {
                    Serial.print(" 🟢 Valid Payload Received (Hex): ");
                    for (size_t i = 0; i < payloadSize - 1; i++)
                    {
                        Serial.print(payload[i], HEX);
                        Serial.print(" ");
                    }
                    Serial.println();
                }
                else
                {
                    Serial.println(" 🔴 [ERROR] Checksum Mismatch: Packet Dropped!");
                }
            }
        }
        else
        {
            Serial.print(" 🔴 [RX] Hardware Error reading data, code: ");
            Serial.println(state);
        }

        radio.startReceive();
    }
}