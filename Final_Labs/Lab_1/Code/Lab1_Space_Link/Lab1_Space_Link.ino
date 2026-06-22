/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 1.4: Space-to-Ground Link (Student Template)
 * Objective: Initialize RF hardware, encapsulate data with KISS, and transmit.
 * Warning: This code must be uploaded directly to the COMMS Subsystem Board (STM32F411RE).
 */

#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

// Link the automated BIST system hidden inside the src directory
#include "src/Lab1_TB_Space_Link.h"

// ====================================================================
// HARDWARE PIN DEFINITIONS (Communication Module Hardware Map)
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
    // TODO: Port your robust KISS byte-stuffing algorithm from Lab 1.3 here.
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
        // TODO 1.2: Set the RF Carrier Frequency to somewhere around 433.0 MHz.

        // TODO 1.3: Set the digital over-the-air Data Rate (Bit Rate) to 9.6 kbps.

        // TODO 1.4: Set the Output Transmit Power level to 2 dBm to avoid overloading.
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
    Serial.setTx(PA2);
    Serial.setRx(PA3);
    Serial.begin(115200);
    delay(2000);

    Serial.println("\n=== FlatSat COMMS: Space-to-Ground Link ===");

    SPI.setMISO(RADIO_MISO);
    SPI.setMOSI(RADIO_MOSI);
    SPI.setSCLK(RADIO_SCK);
    SPI.begin();

    // Run Built-In Self-Test (BIST) Engine
    runSpaceLinkTestbench(radio, initRadioHardware, transmitKISSFrame);
}

void loop()
{
    // --------------------------------------------------------------------
    // TODO 3: Custom Satellite Telemetry Mission Loop
    // --------------------------------------------------------------------
    // Experiment with different data vectors below to see how your Ground Station
    // responds to various real-world space environment scenarios.

    // UNCOMMENT ONE CASE AT A TIME TO TEST YOUR RADIO LINK STACK:

    /* * CASE A: Standard Telemetry Frame (Nominal Health Check)
     * Create an array with a valid XOR checksum.
     * Example: 0x12 XOR 0x34 XOR 0x56 = 0x70
     */
    uint8_t studentData[] = {0x12, 0x34, 0x56, 0x70};
    transmitKISSFrame(radio, studentData, sizeof(studentData));

    /* * CASE B: Extreme Byte-Stuffing Protocol Verification
     * Force the radio to send bytes that match FEND (0xC0) or FESC (0xDB)
     * to ensure your Ported KISS framing protocol successfully escapes them.
     * Correct valid XOR Checksum: 0x05 XOR 0xC0 XOR 0xDB = 0x1E
     */
    // uint8_t studentData[] = {0x05, 0xC0, 0xDB, 0x1E};
    // transmitKISSFrame(radio, studentData, sizeof(studentData));

    /* * CASE C: Anomaly Injection (Solar Storm Event Simulation)
     * Purposely alter or corrupt the final checksum byte to verify if your
     * Ground Station Mission Control parser successfully flags and drops corrupt packages.
     */
    // uint8_t studentData[] = {0x12, 0x34, 0x56, 0xFF}; // 0xFF is an invalid checksum
    // transmitKISSFrame(radio, studentData, sizeof(studentData));

    delay(3000);
}