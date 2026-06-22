// NBSPACE Labs: FlatSat Learning Set
// Lab 3.3: OBC Safe Mode
// Skeleton Code

#include <Arduino.h>
#include <Wire.h>
#include <PCF85063TP.h>
#include "src/Lab3_TB_Safe_Mode.h"

TwoWire I2C_EPS(PF0, PF1);
PCD85063TP rtc;

// -----------------------------------------------
// TODO 1: Fill in sensor I2C addresses and GPIO pins from the hardware table
// Hint: Consult the FlatSat manual or schematic for these specific assignments.
// --- I2C Addresses ---
#define TMP102_ADDRESS         ???  // OBC Board Temperature sensor (via Wire)
#define BATT1_TMP102_ADDRESS   ???  // Battery 1 Temperature sensor (via I2C_EPS)
#define BATT2_TMP102_ADDRESS   ???  // Battery 2 Temperature sensor (via I2C_EPS)

#define INA226_ADDRESS         ???  // Sensor 6: Battery Discharging Monitor (via I2C_EPS)
#define INA226_REG_BUS_VOLTAGE 0x02

// --- Hardware Power Control Pins ---  
#define PIN_COMMS      ???
#define PIN_PAYLOAD_1  ???
#define PIN_PAYLOAD_2  ???

// -----------------------------------------------

float SAFE_MODE_THRESHOLD = 3.3;
bool safeModeTriggered = false;

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  delay(4000);

  Serial.println("\n=== FlatSat Safe Mode Controller Booting ===");

  // Initialize Power Control Pins
  pinMode(PIN_COMMS, OUTPUT);
  pinMode(PIN_PAYLOAD_1, OUTPUT);
  pinMode(PIN_PAYLOAD_2, OUTPUT);

  // Initialize payloads to ON (HIGH)
  digitalWrite(PIN_COMMS, HIGH);
  digitalWrite(PIN_PAYLOAD_1, HIGH);
  digitalWrite(PIN_PAYLOAD_2, HIGH);

  // Initialize Buses
  I2C_EPS.begin();     // EPS I2C on PF0/PF1
  Wire.setSDA(PB9);    // Main I2C on PB9/PB8
  Wire.setSCL(PB8);
  Wire.begin();
  rtc.begin();

  Serial.println("All subsystems initialized. Monitoring power and thermals...");
}

void loop() {

// -----------------------------------------------
  // TODO 2: Complete the I2C transmissions to read sensor data
  // Read bus voltage from INA226 via EPS Bus
  float busVoltage = 0.0;
  I2C_EPS.beginTransmission(???); // Hint: Provide the INA226 device address here
  I2C_EPS.write(???);             // Hint: Provide the INA226 Bus Voltage register address here
  I2C_EPS.endTransmission();
  I2C_EPS.requestFrom(INA226_ADDRESS, 2);
  
  if (I2C_EPS.available() == 2) {
    byte msb = I2C_EPS.read();
    byte lsb = I2C_EPS.read();
    int raw = (msb << 8) | lsb;
    busVoltage = (raw >> 3) * 0.004; // LSB = 4 mV, right-shift 3 bits
  }

  // Read board temperature from TMP102 via Main Wire Bus
  float boardTemp = 0.0;
  Wire.requestFrom(???, 2);       // Hint: Provide the OBC TMP102 address here
  if (Wire.available() == 2) {
    byte msb = Wire.read();
    byte lsb = Wire.read();
    int tempRaw = ((msb << 8) | lsb) >> 4;
    boardTemp = tempRaw * 0.0625; // LSB = 0.0625°C
  }

  // Read Battery 1 temperature from TMP102 via EPS Bus
  float batt1Temp = 0.0;
  I2C_EPS.requestFrom(???, 2);    // Hint: Provide the Battery 1 TMP102 address here
  if (I2C_EPS.available() == 2) {
    byte msb = I2C_EPS.read();
    byte lsb = I2C_EPS.read();
    int tempRaw = ((msb << 8) | lsb) >> 4;
    batt1Temp = tempRaw * 0.0625;
  }

  // Read Battery 2 temperature from TMP102 via EPS Bus
  float batt2Temp = 0.0;
  I2C_EPS.requestFrom(???, 2);    // Hint: Provide the Battery 2 TMP102 address here
  if (I2C_EPS.available() == 2) {
    byte msb = I2C_EPS.read();
    byte lsb = I2C_EPS.read();
    int tempRaw = ((msb << 8) | lsb) >> 4;
    batt2Temp = tempRaw * 0.0625;
  }

  // Build RTC timestamp
  rtc.getTime();
  String timestamp = "";
  timestamp += (rtc.hour < 10)   ? "0" + String(rtc.hour)   : String(rtc.hour);
  timestamp += ":";
  timestamp += (rtc.minute < 10) ? "0" + String(rtc.minute) : String(rtc.minute);
  timestamp += ":";
  timestamp += (rtc.second < 10) ? "0" + String(rtc.second) : String(rtc.second);

  // Safe mode decision logic
  if (busVoltage < SAFE_MODE_THRESHOLD && busVoltage > 0) {
    Serial.println("LOW VOLTAGE! Entering Safe Mode...");
    // TODO 3: Call the function to shut down all non-essential channels
    
    // [Add your code here]
    
    safeModeTriggered = true;
  } else {
    // Log nominal state with all telemetry
    Serial.print("[" + timestamp + "] NOMINAL | ");
    Serial.print("V:" + String(busVoltage,2) + "V | ");
    Serial.print("OBC_T:" + String(boardTemp,2) + "C | ");
    Serial.print("B1_T:" + String(batt1Temp,2) + "C | ");
    Serial.println("B2_T:" + String(batt2Temp,2) + "C");
  }

  // Run external testbench validations
  runSafeModeTestbench(busVoltage, boardTemp, safeModeTriggered, SAFE_MODE_THRESHOLD);

  delay(3000); // Monitoring interval
}

// Disable all non-essential power channels via direct GPIO control
void shutdownAllChannels() {
  // TODO 4: Disable all non-essential power channels
  // Hint: Write LOW to PIN_PAYLOAD_1, PIN_PAYLOAD_2, and PIN_COMMS using digitalWrite.
  
  // [Add your code here]

  Serial.println("All non-essential loads DISABLED via GPIO.");
}