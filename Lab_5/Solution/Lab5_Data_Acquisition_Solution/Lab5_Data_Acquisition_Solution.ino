// NBSPACE Labs: FlatSat Learning Set
// Lab 5.1: Data Acquisition (GPS & RTC)
// Solution Code

#include <Wire.h>
#include <PCF85063TP.h>
#include <TinyGPS++.h>
#include "src/Lab5_TB_Data_Acquisition.h"

// Initialize the GPS UART pins
HardwareSerial gps_uart(PE0, PE1);

// Initialize RTC and GPS objects
PCD85063TP rtc;
TinyGPSPlus gps;

void setup() {
  // Initialize Serial Monitor
  Serial.setTx(PD8);
  Serial.setRx(PD9);
  Serial.begin(115200);

  // Initialize I2C for RTC
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();

  // Start the GPS UART
  gps_uart.begin(9600);

  // Initialize the RTC
  rtc.begin();

  Serial.println("Lab 5.1: GPS and RTC Data Acquisition Started");
}

void loop() {
  // --- Section 1: Read RTC Time ---
  rtc.getTime();

  int hour = rtc.hour;
  int minute = rtc.minute;
  int second = rtc.second;

  // --- Section 2: Read GPS NMEA ---
  while (gps_uart.available() > 0) {
    char c = gps_uart.read();
    gps.encode(c);
  }

  // Print results every second
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    lastPrint = millis();

    Serial.print("Time: ");
    if (hour < 10) Serial.print("0");
    Serial.print(hour); Serial.print(":");
    if (minute < 10) Serial.print("0");
    Serial.print(minute); Serial.print(":");
    if (second < 10) Serial.print("0");
    Serial.print(second);

    if (gps.location.isValid()) {
      Serial.print(" | Location: ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(", ");
      Serial.println(gps.location.lng(), 6);
    } else {
      Serial.println(" | Location: Waiting for GPS signal...");
    }
  }

  runDataAcquisitionTestbench();
}
