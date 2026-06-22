/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 1.3: The KISS Protocol
 * Objective: Implement Byte Stuffing to prepare telemetry data for RF transmission.
 * * TODO: Complete the 'encodeKISS' function body below.
 */

#include <Arduino.h>

#include "src/Lab1_TB_KISS_Protocol.h"

// ====================================================================
// KISS PROTOCOL DEFINITIONS
// ====================================================================
#define FEND 0xC0  // Frame End (Marks start and end of a packet)
#define FESC 0xDB  // Frame Escape (Used to replace forbidden bytes)
#define TFEND 0xDC // Transposed Frame End (Replaces FEND in payload)
#define TFESC 0xDD // Transposed Frame Escape (Replaces FESC in payload)

// --------------------------------------------------------------------
// TODO: KISS Protocol Byte Stuffing Function (STUDENT WORKSPACE)
// --------------------------------------------------------------------
// This function receives a raw telemetry payload and must encapsulate it
// inside the 'outBuffer' using KISS rules (Byte Stuffing).
// Parameters:
//   - payload: Pointer to the raw byte array from sensors
//   - payloadSize: The length of the raw data
//   - outBuffer: The destination array where the KISS packet must be built
// Return:
//   - Must return the final size of the generated KISS packet (including FENDs)
// --------------------------------------------------------------------
size_t encodeKISS(const uint8_t *payload, size_t payloadSize, uint8_t *outBuffer)
{
  size_t outIndex = 0;

  // TODO 1: Add the opening Frame End (FEND) byte to begin the KISS frame.
  // [Write your code here]

  // TODO 2: Loop through the raw payload and apply the Byte Stuffing rules.
  // Iterate from i = 0 to payloadSize - 1:
  //   - If current byte equals FEND (0xC0), replace it with FESC followed by TFEND.
  //   - If current byte equals FESC (0xDB), replace it with FESC followed by TFESC.
  //   - Otherwise, it is a normal sensor byte, copy it directly to outBuffer as-is.
  // Remember to increment your 'outIndex' tracking variable properly!
  // [Write your packet stuffing loop logic here]

  // TODO 3: Add the closing Frame End (FEND) byte to complete the KISS frame.
  // [Write your code here]

  // TODO 4: Return the total number of bytes written to the outBuffer.
  return 0; // Change this to return your calculated packet size tracking variable
}

void setup()
{
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);

  delay(2000);

  Serial.println("\n=== FlatSat COMMS: KISS Protocol Encapsulation ===");

  // --------------------------------------------------------------------
  // Run Testbench (TB)
  // --------------------------------------------------------------------
  runKISSTestbench(encodeKISS);
}

void loop()
{
  // Main operational pipeline executes once during hardware setup loop
}