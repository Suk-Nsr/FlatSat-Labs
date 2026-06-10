/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 4.2: Space-to-Ground Link (Student Template)
 * Objective: Initialize RF hardware, encapsulate data with KISS, and transmit.
 * Warning: This code must be uploaded directly to the COMMS Subsystem Board (STM32F411RE).
 */

#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

// Link the automated BIST system hidden inside the src directory
#include "src/Lab4_TB_Space_Link.h"

// ====================================================================
// HARDWARE PIN DEFINITIONS (Communication Module Hardware Map)
// ====================================================================
// SPI Bus Pins for RF Transceiver
#define RADIO_SCK PA5
#define RADIO_MISO PA6
#define RADIO_MOSI PA7

// Hardware Control Pins
#define RADIO_NSS PB6
#define RADIO_DIO0 PA10
#define RADIO_RESET PC7
#define RADIO_DIO1 -1

// Instantiate the SX1278 radio module object explicitly mapped to standard SPI bus
SX1278 radio = new Module(RADIO_NSS, RADIO_DIO0, RADIO_RESET, RADIO_DIO1, SPI);

// ====================================================================
// KISS PROTOCOL DEFINITIONS
// ====================================================================
#define FEND 0xC0
#define FESC 0xDB
#define TFEND 0xDC
#define TFESC 0xDD

// --------------------------------------------------------------------
// KISS Protocol Byte Stuffing Function (STUDENT TEMPLATE)
// --------------------------------------------------------------------
size_t encodeKISS(const uint8_t *payload, size_t payloadSize, uint8_t *outBuffer)
{
    // TODO: Port your robust KISS byte-stuffing algorithm from Lab 4.1 here.
    // Since the frame structure requirements are identical, you can reuse your previous implementation.

    return 0; // Return the final packet size generated
}

// --------------------------------------------------------------------
// TODO 1: Radio Hardware Initialization (STUDENT TEMPLATE)
// --------------------------------------------------------------------
int initRadioHardware(SX1278 &radioObj)
{
    // TODO 1.1: Initialize the radio transceiver using FSK modulation mode.
    // Hint: Invoke the .beginFSK() method on the radioObj and capture its return code.
    int state = -1;

    if (state == RADIOLIB_ERR_NONE)
    {
        // TODO 1.2: Set the RF Carrier Frequency to exactly 433.0 MHz.

        // TODO 1.3: Set the digital over-the-air Data Rate (Bit Rate) to 9.6 kbps.

        // TODO 1.4: Set the Output Transmit Power level to 10 dBm to avoid overloading.
    }
    return state; // Return the initialization status code back to the testbench
}

// --------------------------------------------------------------------
// TODO 2: Data Encapsulation & Transmission (STUDENT TEMPLATE)
// --------------------------------------------------------------------
int transmitKISSFrame(SX1278 &radioObj, const uint8_t *payload, size_t size)
{
    uint8_t txBuffer[64];
    size_t packetSize = 0;

    // TODO 2.1: Encapsulate the raw payload buffer into txBuffer using the encodeKISS function.
    // Make sure to assign the resulting frame length to the 'packetSize' variable.

    // TODO 2.2: Loop through txBuffer and print the hex bytes to the Serial Monitor.
    // Example Output format: "Transmitting RF Packet: C0 AA DB DC BB C0 "

    // TODO 2.3: Blatantly transmit the compiled KISS frame packet over the airwaves.
    // Hint: Invoke radioObj.transmit(...) with your buffer and its actual calculated size.
    return -1; // Return the transmission execution status code back to the testbench
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

    // --------------------------------------------------------------------
    // Run Built-In Self-Test (BIST) Engine
    // The testbench will call your custom initialization and transmission pointers.
    // --------------------------------------------------------------------
    runSpaceLinkTestbench(radio, initRadioHardware, transmitKISSFrame);
}

void loop()
{
    // System operational flow wraps inside the one-shot BIST execution
}