/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 3.3: OBC Safe Mode
 * Objective: Implement autonomous safe mode that triggers when
 *            battery voltage drops below a critical threshold.
 */

#include <Arduino.h>
#include <Wire.h>
#include <PCF85063TP.h>
#include "testbench.h"

TwoWire I2C_EPS(PF0, PF1);
PCD85063TP rtc;

// ====================================================================
// HARDWARE DEFINITIONS (Pre-configured from previous labs)
// ====================================================================
#define TMP102_ADDRESS         0x4A
#define INA219_ADDRESS         0x40
#define INA219_REG_BUS_VOLTAGE 0x02
#define EPS_CONTROLLER_ADDRESS 0x08
#define EPS_CMD_CH1_DISABLE    0x22
#define EPS_CMD_CH2_DISABLE    0x32
#define EPS_CMD_CH3_DISABLE    0x42

// ====================================================================
// SAFE MODE CONFIGURATION
// ====================================================================

// TODO 1: Define the battery voltage threshold for triggering Safe Mode.
// Replace 0.0 with a reasonable threshold (e.g., 3.3 for a single LiPo cell).
// Below this voltage, the OBC should enter safe mode to protect the battery.
float SAFE_MODE_THRESHOLD = 0.0;

bool safeModeTriggered = false;

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  while (!Serial) {;}

  Serial.println("\n=== FlatSat Safe Mode Controller Booting ===");

  I2C_EPS.begin();
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();
  rtc.begin();

  Serial.println("All subsystems initialized. Monitoring power...");
}

void loop() {
  // ---------------------------------------------------------
  // Step 1: Read bus voltage from INA219 (Pre-filled from Lab 3.1)
  // ---------------------------------------------------------
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

  // ---------------------------------------------------------
  // Step 2: Read temperature from TMP102 (Pre-filled from Lab 1.3)
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
  // Step 3: Get timestamp (Pre-filled from Lab 1.3)
  // ---------------------------------------------------------
  rtc.getTime();
  String timestamp = "";
  timestamp += (rtc.hour < 10)   ? "0" + String(rtc.hour)   : String(rtc.hour);
  timestamp += ":";
  timestamp += (rtc.minute < 10) ? "0" + String(rtc.minute) : String(rtc.minute);
  timestamp += ":";
  timestamp += (rtc.second < 10) ? "0" + String(rtc.second) : String(rtc.second);

  // ---------------------------------------------------------
  // Step 4: Safe Mode Decision -> [TODO]
  // ---------------------------------------------------------

  // TODO 2: Implement the safe mode decision logic.
  // If busVoltage is BELOW SAFE_MODE_THRESHOLD AND busVoltage is greater than 0:
  //   a) Print: "LOW VOLTAGE! Entering Safe Mode..."
  //   b) Call: shutdownAllChannels()
  //   c) Set: safeModeTriggered = true
  // Else:
  //   Print normal telemetry, for example:
  //   Serial.println("[" + timestamp + "] NOMINAL | V:" + String(busVoltage,2) + "V T:" + String(boardTemp,2) + "C");
  // [Add your code here]
  


  runSafeModeTestbench(busVoltage, boardTemp, safeModeTriggered, SAFE_MODE_THRESHOLD);

  delay(3000);
}

// ====================================================================
// HELPER: Shutdown All Power Channels (Pre-filled from Lab 3.2)
// ====================================================================
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
