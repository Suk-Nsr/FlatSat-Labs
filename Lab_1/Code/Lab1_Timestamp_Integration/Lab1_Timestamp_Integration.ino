/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 1.3: Timestamp Integration
 */

#include <Arduino.h>
#include <Wire.h>
#include <PCF85063TP.h> 
#include "testbench.h"

TwoWire I2C_EPS(PF0, PF1);
PCD85063TP rtc; 

// TODO 1: Find the correct I2C address for the TMP102 Temperature Sensor.
// Replace 0x00 with the correct hexadecimal address (e.g., 0x48, 0x4A, etc.)
#define TMP102_ADDRESS 0x00 

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);

  Serial.begin(115200);
  
  while (!Serial) {
    ;
  }
  
  // I2C and RTC Initialization
  I2C_EPS.begin();
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin(); 
  rtc.begin();
  
  Serial.println("OBC Telemetry System Started...");
}

void loop() {
  // Read Time Data
  rtc.getTime(); 
  int h = rtc.hour;
  int m = rtc.minute;
  int s = rtc.second;

  float boardTemp = 0.0;

  // ---------------------------------------------------------
  // Part 2: Read Temperature Sensor -> [TODO]
  // ---------------------------------------------------------
  // Task: Request 2 bytes of data from the TMP102 sensor via I2C_EPS,
  // read the MSB and LSB, and calculate the final temperature in Celsius.
  
  // TODO 3: Write your sensor reading logic below and assign the result to boardTemp
  // [Add your code here]
  
  
  

  // ---------------------------------------------------------
  // Part 3: Data Integration (Do not touch this part)
  // ---------------------------------------------------------

  String hh = (h < 10) ? "0" + String(h) : String(h);
  String mm = (m < 10) ? "0" + String(m) : String(m);
  String ss = (s < 10) ? "0" + String(s) : String(s);
  
  String tempString = String(boardTemp, 2);
  
  String telemetryPacket = "[" + hh + ":" + mm + ":" + ss + "] OBC Temperature: " + tempString + " °C";

  runTelemetryTestbench(telemetryPacket, h, m, s, boardTemp);

  delay(1000);
}