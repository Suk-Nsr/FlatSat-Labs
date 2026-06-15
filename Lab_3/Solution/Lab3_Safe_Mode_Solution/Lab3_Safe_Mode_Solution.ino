// NBSPACE Labs: FlatSat Learning Set
// Lab 3.3: OBC Safe Mode
// Solution Code

#include <Arduino.h>
#include <Wire.h>
#include <PCF85063TP.h>
#include "src/Lab3_TB_Safe_Mode.h"

TwoWire I2C_EPS(PF0, PF1);
PCD85063TP rtc;

// TODO 1 (FILLED): Sensor and EPS controller I2C addresses
#define TMP102_ADDRESS         0x4A  // Temperature sensor
#define INA219_ADDRESS         0x40  // Bus voltage/current monitor
#define INA219_REG_BUS_VOLTAGE 0x02
#define EPS_CONTROLLER_ADDRESS 0x08  // EPS MCU (channel control)
#define EPS_CMD_CH1_DISABLE    0x22  // Payload 1 OFF
#define EPS_CMD_CH2_DISABLE    0x32  // Payload 2 OFF
#define EPS_CMD_CH3_DISABLE    0x42  // COMMS OFF

float SAFE_MODE_THRESHOLD = 3.3;

bool safeModeTriggered = false;

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  delay(4000);

  Serial.println("\n=== FlatSat Safe Mode Controller Booting ===");

  I2C_EPS.begin();
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();
  rtc.begin();

  Serial.println("All subsystems initialized. Monitoring power...");
}

void loop() {
  // Read bus voltage from INA219
  float busVoltage = 0.0;
  I2C_EPS.beginTransmission(INA219_ADDRESS);
  I2C_EPS.write(INA219_REG_BUS_VOLTAGE);
  I2C_EPS.endTransmission();
  I2C_EPS.requestFrom(INA219_ADDRESS, 2);
  if (I2C_EPS.available() == 2) {
    byte msb = I2C_EPS.read();
    byte lsb = I2C_EPS.read();
    int raw = (msb << 8) | lsb;
    busVoltage = (raw >> 3) * 0.004; // LSB = 4 mV, right-shift 3 bits
  }

  // Read board temperature from TMP102
  float boardTemp = 0.0;
  I2C_EPS.requestFrom(TMP102_ADDRESS, 2);
  if (I2C_EPS.available() == 2) {
    byte msb = I2C_EPS.read();
    byte lsb = I2C_EPS.read();
    int tempRaw = ((msb << 8) | lsb) >> 4;
    boardTemp = tempRaw * 0.0625; // LSB = 0.0625°C
  }

  // Build RTC timestamp
  rtc.getTime();
  String timestamp = "";
  timestamp += (rtc.hour < 10)   ? "0" + String(rtc.hour)   : String(rtc.hour);
  timestamp += ":";
  timestamp += (rtc.minute < 10) ? "0" + String(rtc.minute) : String(rtc.minute);
  timestamp += ":";
  timestamp += (rtc.second < 10) ? "0" + String(rtc.second) : String(rtc.second);

  // TODO 3 (FILLED): Implement safe mode decision logic
  if (busVoltage < SAFE_MODE_THRESHOLD && busVoltage > 0) {
    Serial.println("LOW VOLTAGE! Entering Safe Mode...");
    shutdownAllChannels();
    safeModeTriggered = true;
  } else {
    Serial.println("[" + timestamp + "] NOMINAL | V:" + String(busVoltage,2) + "V T:" + String(boardTemp,2) + "C");
  }

  runSafeModeTestbench(busVoltage, boardTemp, safeModeTriggered, SAFE_MODE_THRESHOLD);

  delay(3000); // Monitoring interval
}

// Disable all non-essential power channels via EPS controller
void shutdownAllChannels() {
  I2C_EPS.beginTransmission(EPS_CONTROLLER_ADDRESS);
  I2C_EPS.write(EPS_CMD_CH1_DISABLE);
  I2C_EPS.endTransmission();

  I2C_EPS.beginTransmission(EPS_CONTROLLER_ADDRESS);
  I2C_EPS.write(EPS_CMD_CH2_DISABLE);
  I2C_EPS.endTransmission();

  I2C_EPS.beginTransmission(EPS_CONTROLLER_ADDRESS);
  I2C_EPS.write(EPS_CMD_CH3_DISABLE);
  I2C_EPS.endTransmission();

  Serial.println("All non-essential loads DISABLED.");
}
