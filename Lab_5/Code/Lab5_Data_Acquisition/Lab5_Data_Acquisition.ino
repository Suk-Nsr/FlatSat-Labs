// NBSPACE Labs: FlatSat Learning Set
// Lab 5.1: Data Acquisition (GPS & RTC)
// Objective: Learn to read and parse NMEA coordinates from GPS and time from RTC.

#include <Wire.h>
#include <PCF85063TP.h>
#include "src/Lab5_TB_Data_Acquisition.h"

// TODO 1: Initialize the GPS UART pins (from OBC documentation: PE0, PE1)
HardwareSerial gps_uart(???, ???);

// Initialize RTC
PCD85063TP rtc;

void setup() {
  // Initialize Serial Monitor
  Serial.setTx(PD8);
  Serial.setRx(PD9);
  Serial.begin(115200);

  // Initialize I2C for RTC
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();

  // TODO 2: Start the GPS UART with a baud rate of 9600
  // [Add your code here]

  // TODO 3: Initialize the RTC
  // [Add your code here]

  Serial.println("Lab 5.1: GPS and RTC Data Acquisition Started");
}

void loop() {
  // --- Section 1: Read RTC Time ---
  // TODO 4: Read the current time from the RTC module
  // [Add your code here]

  int hour = rtc.hour;
  int minute = rtc.minute;
  int second = rtc.second;

  Serial.print("Time: ");
  Serial.print(hour); Serial.print(":");
  Serial.print(minute); Serial.print(":");
  Serial.println(second);

  // --- Section 2: Read GPS NMEA ---
  // TODO 5: Read data from the GPS UART and print it to the Serial Monitor
  // Hint: Use gps_uart.available() and gps_uart.read()
  
  while (???) {
    char c = ???;
    Serial.write(c);
  }

  // Note for students: 
  // In the solution, you may use a library like TinyGPS++ to easily parse the NMEA 
  // data into Latitude and Longitude values. For this step, simply verifying 
  // that you can read the raw $GPRMC or $GPGGA sentences is sufficient.

  runDataAcquisitionTestbench();
  delay(1000);
}
