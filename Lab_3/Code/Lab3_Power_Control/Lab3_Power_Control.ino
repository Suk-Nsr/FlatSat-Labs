/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 3.2: OBC Power Control
 * Objective: Send I2C commands to the EPS to enable/disable power channels.
 */

#include <Arduino.h>
#include <Wire.h>
#include "testbench.h"

TwoWire I2C_EPS(PF0, PF1);

// ====================================================================
// HARDWARE DEFINITIONS
// ====================================================================
#define EPS_CONTROLLER_ADDRESS 0x08

#define EPS_CMD_CH1_ENABLE     0x21  // Payload Bus ON
#define EPS_CMD_CH1_DISABLE    0x22  // Payload Bus OFF
#define EPS_CMD_CH2_ENABLE     0x31  // COMMS Bus ON
#define EPS_CMD_CH2_DISABLE    0x32  // COMMS Bus OFF
#define EPS_CMD_CH3_ENABLE     0x41  // ADCS Bus ON
#define EPS_CMD_CH3_DISABLE    0x42  // ADCS Bus OFF

bool ch1_payload = false;
bool ch2_comms   = false;
bool ch3_adcs    = false;

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  while (!Serial) {;}

  Serial.println("\n=== FlatSat Power Control System Booting ===");

  I2C_EPS.begin();
  Serial.println("EPS I2C command bus initialized.");

  // ---------------------------------------------------------
  // EXAMPLE: How to DISABLE Channel 3 (ADCS Bus)
  // This is pre-filled to show you the I2C command pattern.
  // ---------------------------------------------------------
  I2C_EPS.beginTransmission(EPS_CONTROLLER_ADDRESS);
  I2C_EPS.write(EPS_CMD_CH3_DISABLE);
  I2C_EPS.endTransmission();
  ch3_adcs = false;
  Serial.println("Command sent: ADCS Bus -> OFF");

  // ---------------------------------------------------------
  // TODO 1: ENABLE Channel 1 (Payload Bus)
  // ---------------------------------------------------------
  // Follow the same pattern as the example above, but use
  // EPS_CMD_CH1_ENABLE instead, and set ch1_payload = true.
  // [Add your code here]
  


  // ---------------------------------------------------------
  // TODO 2: ENABLE Channel 2 (COMMS Bus)
  // ---------------------------------------------------------
  // Same pattern again, use EPS_CMD_CH2_ENABLE
  // and set ch2_comms = true.
  // [Add your code here]
  


  // ---------------------------------------------------------
  // Power Status Display (Do not modify)
  // ---------------------------------------------------------
  Serial.println("\n--- Power Channel Status ---");
  Serial.print("  CH1 [Payload]: "); Serial.println(ch1_payload ? "ON" : "OFF");
  Serial.print("  CH2 [COMMS]  : "); Serial.println(ch2_comms   ? "ON" : "OFF");
  Serial.print("  CH3 [ADCS]   : "); Serial.println(ch3_adcs    ? "ON" : "OFF");
  Serial.println("-----------------------------------");

  runPowerControlTestbench(ch1_payload, ch2_comms, ch3_adcs);
}

void loop() {
  // Power commands are issued once at boot
}
