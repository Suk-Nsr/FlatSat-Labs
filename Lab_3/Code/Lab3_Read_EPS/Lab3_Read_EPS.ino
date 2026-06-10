/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 3.1: OBC Read EPS Telemetry
 * Objective: Read battery bus voltage from the INA219 Power Monitor via I2C.
 */

#include <Arduino.h>
#include <Wire.h>
#include <PCF85063TP.h>
#include "src/Lab3_TB_Read_EPS.h"

TwoWire I2C_EPS(PF0, PF1);
PCD85063TP rtc;

#define TMP102_ADDRESS 0x4A

// TODO 1: Find the correct I2C address for the INA219 Power Monitor.
// Replace 0x00 with the correct hexadecimal address (e.g., 0x40, 0x41, etc.)
// Hint: Use your I2C Scanner from Lab 1.2 to discover it on the EPS bus!
#define INA219_ADDRESS 0x00

#define INA219_REG_BUS_VOLTAGE 0x02

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);

  delay(4000);

  Serial.println("\n=== FlatSat EPS Telemetry Reader Booting ===");

  I2C_EPS.begin();
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();
  rtc.begin();

  Serial.println("EPS telemetry link established.");
}

void loop() {
  // ---------------------------------------------------------
  // Part 1: Read Timestamp from RTC (Pre-filled from Lab 1.3)
  // ---------------------------------------------------------
  rtc.getTime();
  int h = rtc.hour;
  int m = rtc.minute;
  int s = rtc.second;

  // ---------------------------------------------------------
  // Part 2: Read Board Temperature (Pre-filled from Lab 1.3)
  // ---------------------------------------------------------
  float boardTemp = 0.0;
  I2C_EPS.requestFrom(TMP102_ADDRESS, 2);
  if (I2C_EPS.available() == 2) {
    byte msb = I2C_EPS.read();
    byte lsb = I2C_EPS.read();
    int tempRaw = ((msb << 8) | lsb) >> 4;
    boardTemp = tempRaw * 0.0625;
  }

  // ---------------------------------------------------------
  // Part 3: Read Bus Voltage from INA219 -> [TODO]
  // ---------------------------------------------------------
  float busVoltage = 0.0;

  // TODO 2: Read the bus voltage from the INA219 Power Monitor.
  // Follow these steps:
  //   I2C_EPS.beginTransmission(INA219_ADDRESS);
  //   I2C_EPS.write(INA219_REG_BUS_VOLTAGE);
  //   I2C_EPS.endTransmission();
  //   I2C_EPS.requestFrom(INA219_ADDRESS, 2);
  //   Then read MSB and LSB, combine: (msb << 8) | lsb
  //   Shift right by 3, multiply by 0.004 to get Volts.
  // [Add your code here]
  


  // ---------------------------------------------------------
  // Part 4: Data Integration (Do not modify this section)
  // ---------------------------------------------------------
  String hh = (h < 10) ? "0" + String(h) : String(h);
  String mm = (m < 10) ? "0" + String(m) : String(m);
  String ss = (s < 10) ? "0" + String(s) : String(s);

  String telemetryPacket = "[" + hh + ":" + mm + ":" + ss + "] ";
  telemetryPacket += "TEMP:" + String(boardTemp, 2) + "C | ";
  telemetryPacket += "VBUS:" + String(busVoltage, 2) + "V";

  Serial.println(telemetryPacket);

  runEPSTestbench(boardTemp, busVoltage, INA219_ADDRESS);

  delay(2000);
}
