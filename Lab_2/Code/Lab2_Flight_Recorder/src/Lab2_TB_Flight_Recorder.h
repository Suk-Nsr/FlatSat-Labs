#ifndef TESTBENCH_H
#define TESTBENCH_H

#include <Arduino.h>

#define EXTERNAL_FLASH_USE_SPI 1
#include <Adafruit_SPIFlash.h>

// Initailize Testbench for file .ino
void runFlightRecorderTestbench(Adafruit_SPIFlash& flashObj, uint32_t studentId, uint8_t studentCount);

#endif // TESTBENCH_H