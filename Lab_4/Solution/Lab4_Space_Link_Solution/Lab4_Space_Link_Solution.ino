/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 4.2: Space-to-Ground Link (Solution Key)
 * Objective: Initialize RF hardware, encapsulate data with KISS, and transmit.
 */

#include <Arduino.h>
#include <RadioLib.h>

// Link the automated BIST system hidden inside the src directory
#include "src/Lab4_TB_Space_Link.h"

// ====================================================================
// HARDWARE PIN DEFINITIONS (Communication Module)
// ====================================================================
// SPI Bus Pins for Radio
#define RADIO_SCK PA5
#define RADIO_MISO PA6
#define RADIO_MOSI PA7

// Control Pins
#define RADIO_NSS PB6
#define RADIO_DIO0 PA10
#define RADIO_RESET PC7
#define RADIO_DIO1 -1

// Instantiate the SX1278 radio module object
SX1278 radio = new Module(RADIO_NSS, RADIO_DIO0, RADIO_RESET, RADIO_DIO1);

// ====================================================================
// KISS PROTOCOL ENCODER (From Lab 4.1)
// ====================================================================
#define FEND 0xC0
#define FESC 0xDB
#define TFEND 0xDC
#define TFESC 0xDD

size_t encodeKISS(const uint8_t *payload, size_t payloadSize, uint8_t *outBuffer)
{
    size_t outIndex = 0;
    outBuffer[outIndex++] = FEND;
    for (size_t i = 0; i < payloadSize; i++)
    {
        uint8_t currentByte = payload[i];
        if (currentByte == FEND)
        {
            outBuffer[outIndex++] = FESC;
            outBuffer[outIndex++] = TFEND;
        }
        else if (currentByte == FESC)
        {
            outBuffer[outIndex++] = FESC;
            outBuffer[outIndex++] = TFESC;
        }
        else
        {
            outBuffer[outIndex++] = currentByte;
        }
    }
    outBuffer[outIndex++] = FEND;
    return outIndex;
}

// ---------------------------------------------------------
// TODO 1: Radio Hardware Initialization (FILLED SOLUTION)
// ---------------------------------------------------------
int initRadioHardware(SX1278 &radioObj)
{
    // 1. Begin FSK modulation mode
    int state = radioObj.beginFSK();

    if (state == RADIOLIB_ERR_NONE)
    {
        // 2. Set Carrier Frequency to 433.0 MHz
        radioObj.setFrequency(433.0);

        // 3. Set Data Rate (Bit Rate) to 9.6 kbps
        radioObj.setBitRate(9.6);

        // 4. Set Transmit Power to 10 dBm
        radioObj.setOutputPower(10);
    }
    return state; // Return the initialization status code
}

// ---------------------------------------------------------
// TODO 2: Data Encapsulation & Transmission (FILLED SOLUTION)
// ---------------------------------------------------------
int transmitKISSFrame(SX1278 &radioObj, const uint8_t *payload, size_t size)
{
    uint8_t txBuffer[64];

    // 1. Encapsulate the payload using the KISS protocol
    size_t packetSize = encodeKISS(payload, size, txBuffer);

    Serial.print("Transmitting RF Packet: ");
    for (size_t i = 0; i < packetSize; i++)
    {
        Serial.print(txBuffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    // 2. Transmit the packet over the air and return the status
    return radioObj.transmit(txBuffer, packetSize);
}

void setup()
{
    // Initialize standard hardware Serial for debug printing
    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }

    Serial.println("\n=== FlatSat COMMS: Space-to-Ground Link ===");

    // Force hardware SPI bus remapping onto the specific COMMS module track tracing
    SPI.setMISO(RADIO_MISO);
    SPI.setMOSI(RADIO_MOSI);
    SPI.setSCLK(RADIO_SCK);
    SPI.begin();

    // ---------------------------------------------------------
    // Run Built-In Self-Test (BIST)
    // ---------------------------------------------------------
    runSpaceLinkTestbench(radio, initRadioHardware, transmitKISSFrame);
}

void loop()
{
    // Telemetry transmission handled in BIST or custom loop logic later
}