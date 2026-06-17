// NBSPACE Labs: FlatSat Learning Set
// Lab 5.1: Data Acquisition (GPS & RTC)
// Skeleton Code

#include <Wire.h>
#include <PCF85063TP.h>
#include <TinyGPS++.h>

// Initialize the GPS UART pins
// TODO 1: Declare the GPS UART with its pins as a HardwareSerial object named gps_uart
// [Add your code here]

// Initialize RTC and GPS objects
PCD85063TP rtc;

// TODO 2: Initialize the GPS as a TinyGPSPlus object named gps
// [Add your code here]

void setup() {
  // Initialize Serial Monitor
  Serial.setTx(PD8);
  Serial.setRx(PD9);
  Serial.begin(115200);

  // Initialize I2C for RTC
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();

  // Start the GPS UART (TODO 3)
  // TODO 3: Begin the GPS UART communication at 9600 baud rate
  // [Add your code here]

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
  // Read all incoming GPS data
  while (gps_uart.available() > 0) {
    char c = gps_uart.read();
    gps.encode(c);
  }

  // Print results every second
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    lastPrint = millis();

    if (!gps.location.isValid()) {
      Serial.println("Waiting for GPS fix...");
      return;
    }

    // 1. Print Time
    Serial.print("Time: ");
    if (hour < 10) Serial.print("0");
    Serial.print(hour); Serial.print(":");
    if (minute < 10) Serial.print("0");
    Serial.print(minute); Serial.print(":");
    if (second < 10) Serial.print("0");
    Serial.print(second);

    // 2. Print Location
    Serial.print(" | Location: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(", ");
    Serial.println(gps.location.lng(), 6);
  }
}