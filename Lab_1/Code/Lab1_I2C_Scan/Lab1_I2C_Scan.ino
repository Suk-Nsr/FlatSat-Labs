
// NBSPACE Labs: FlatSat Learning Set
// Lab 1.2: Dual I2C Scanner
// Objective: Scan for I2C Addresses on BOTH subsystem buses (EPS and Main).


#include <Wire.h>
#include "src/Lab1_TB_I2C_Scan.h"

// TODO 1: Find the EPS I2C pins (SDA,SCL)from the documentation and configure the TwoWire object.
TwoWire I2C_EPS(???, ???);

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  
  delay(4000);

  Serial.println("\n--- FlatSat Dual I2C Scanner ---");

  // TODO 2: Initialize the EPS I2C bus (I2C_EPS).
  // [Add your code here]
  

  // TODO 3: Find the Main I2C pins from the documentation and configure the Wire object.
  Wire.setSDA(???);
  Wire.setSCL(???);
  
  // TODO 4: Initialize the Main I2C bus (Wire).
  // [Add your code here]
  
  byte error_eps, error_main, address;
  bool eps_found[128] = {false};
  bool main_found[128] = {false};

  Serial.println("Scanning both I2C Buses...");

  // TODO 5: Write the loop condition to scan all possible I2C addresses (1 to 127).
  for (???) {
    I2C_EPS.beginTransmission(address);
    error_eps = I2C_EPS.endTransmission();

    if (error_eps == 0) {
      Serial.print("Found device on [EPS Bus]  at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      eps_found[address] = true;
    }

    Wire.beginTransmission(address);
    error_main = Wire.endTransmission();

    if (error_main == 0) {
      Serial.print("Found device on [Main Bus] at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      main_found[address] = true;
    }
  } 

  runI2CScanTestbench(eps_found, main_found);
}

void loop() {
  
}