/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 3.2: OBC Power Control (Solution)
 */

#include <Arduino.h>
#include <Wire.h>

TwoWire I2C_EPS(PF0, PF1);

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

  // EXAMPLE: Disable Channel 3 (ADCS)
  I2C_EPS.beginTransmission(EPS_CONTROLLER_ADDRESS);
  I2C_EPS.write(EPS_CMD_CH3_DISABLE);
  I2C_EPS.endTransmission();
  ch3_adcs = false;
  Serial.println("Command sent: ADCS Bus -> OFF");

  // TODO 1 (FILLED): Enable Channel 1 (Payload)
  I2C_EPS.beginTransmission(EPS_CONTROLLER_ADDRESS);
  I2C_EPS.write(EPS_CMD_CH1_ENABLE);
  I2C_EPS.endTransmission();
  ch1_payload = true;
  Serial.println("Command sent: Payload Bus -> ON");

  // TODO 2 (FILLED): Enable Channel 2 (COMMS)
  I2C_EPS.beginTransmission(EPS_CONTROLLER_ADDRESS);
  I2C_EPS.write(EPS_CMD_CH2_ENABLE);
  I2C_EPS.endTransmission();
  ch2_comms = true;
  Serial.println("Command sent: COMMS Bus -> ON");

  Serial.println("\n--- Power Channel Status ---");
  Serial.print("  CH1 [Payload]: "); Serial.println(ch1_payload ? "ON" : "OFF");
  Serial.print("  CH2 [COMMS]  : "); Serial.println(ch2_comms   ? "ON" : "OFF");
  Serial.print("  CH3 [ADCS]   : "); Serial.println(ch3_adcs    ? "ON" : "OFF");
  Serial.println("-----------------------------------");
}

void loop() {
}
