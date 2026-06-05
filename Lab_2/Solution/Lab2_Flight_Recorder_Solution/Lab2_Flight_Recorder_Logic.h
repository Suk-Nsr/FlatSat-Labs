/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 2.1: Flight Recorder Logic Module (Solution Core)
 */

#ifndef LAB2_FLIGHT_RECORDER_LOGIC_H
#define LAB2_FLIGHT_RECORDER_LOGIC_H

// Core processing algorithm using cross-file references (&)
void executeFlightRecorderLogic(Adafruit_SPIFlash& flashObj, uint32_t& jedecId, uint8_t& currentBootCount) {
  uint8_t readValue = 0;

  // TODO 1: Read the JEDEC ID from the Flash memory and save it into the 'jedecId' reference.
  jedecId = flashObj.getJEDECID();

  Serial.print("SPI Flash JEDEC ID: 0x");
  Serial.println(jedecId, HEX);

  // TODO 2: Read 1 byte of data from Flash address 0x0000 and store it in 'readValue'.
  flashObj.readBuffer(0x0000, &readValue, 1);

  // TODO 3: Check if Flash is empty (0xFF) or has previous boot logs.
  if (readValue == 0xFF) {
    currentBootCount = 0; // Factory state or freshly reset
  } else {
    currentBootCount = readValue + 1; // Increment system cycle
  }

  Serial.print("System is booting for the [ ");
  Serial.print(currentBootCount);
  Serial.println(" ] time(s).");

  // TODO 4: Rule of Flash memory -> You MUST erase the sector before writing!
  // Erasing sector 0 clears both data (0x0000) and our injected dirty byte (0x0001).
  flashObj.eraseSector(0);

  // TODO 5: Save the updated 'currentBootCount' back to Flash at address 0x0000.
  flashObj.writeBuffer(0x0000, &currentBootCount, 1);
}

#endif // LAB2_FLIGHT_RECORDER_LOGIC_H