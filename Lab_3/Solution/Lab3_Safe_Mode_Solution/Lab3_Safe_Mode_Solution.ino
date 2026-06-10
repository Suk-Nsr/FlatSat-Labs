/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 3.3: OBC Safe Mode (Solution)
 */

#include <Arduino.h>
#include <Wire.h>
#include <PCF85063TP.h>

TwoWire I2C_EPS(PF0, PF1);
PCD85063TP rtc;

#define TMP102_ADDRESS         0x4A
#define INA219_ADDRESS         0x40
#define INA219_REG_BUS_VOLTAGE 0x02
#define EPS_CONTROLLER_ADDRESS 0x08
#define EPS_CMD_CH1_DISABLE    0x22
#define EPS_CMD_CH2_DISABLE    0x32
#define EPS_CMD_CH3_DISABLE    0x42

// TODO 1 (FILLED): Set Threshold
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
  float busVoltage = 0.0;
  I2C_EPS.beginTransmission(INA219_ADDRESS);
  I2C_EPS.write(INA219_REG_BUS_VOLTAGE);
  I2C_EPS.endTransmission();
  I2C_EPS.requestFrom(INA219_ADDRESS, 2);
  if (I2C_EPS.available() == 2) {
    byte msb = I2C_EPS.read();
    byte lsb = I2C_EPS.read();
    int raw = (msb << 8) | lsb;
    busVoltage = (raw >> 3) * 0.004;
  }

  float boardTemp = 0.0;
  I2C_EPS.requestFrom(TMP102_ADDRESS, 2);
  if (I2C_EPS.available() == 2) {
    byte msb = I2C_EPS.read();
    byte lsb = I2C_EPS.read();
    int tempRaw = ((msb << 8) | lsb) >> 4;
    boardTemp = tempRaw * 0.0625;
  }

  rtc.getTime();
  String timestamp = "";
  timestamp += (rtc.hour < 10)   ? "0" + String(rtc.hour)   : String(rtc.hour);
  timestamp += ":";
  timestamp += (rtc.minute < 10) ? "0" + String(rtc.minute) : String(rtc.minute);
  timestamp += ":";
  timestamp += (rtc.second < 10) ? "0" + String(rtc.second) : String(rtc.second);

  // TODO 2 (FILLED): Implement safe mode logic
  if (busVoltage < SAFE_MODE_THRESHOLD && busVoltage > 0) {
    Serial.println("LOW VOLTAGE! Entering Safe Mode...");
    shutdownAllChannels();
    safeModeTriggered = true;
  } else {
    Serial.println("[" + timestamp + "] NOMINAL | V:" + String(busVoltage,2) + "V T:" + String(boardTemp,2) + "C");
  }

  delay(3000);
}

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
