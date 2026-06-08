
// NBSPACE Labs: FlatSat Learning Set
// Lab 1.2: Dual I2C Scanner 
// (Solution Code)

#include <Wire.h>
#include "src/Lab1_TB_I2C_Scan.h"


// TODO 1 (FILLED)
TwoWire I2C_EPS(PF0, PF1);

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  
  while (!Serial){;}

  Serial.println("\n--- FlatSat Dual I2C Scanner ---");
  
  // TODO 2 (FILLED)
  I2C_EPS.begin();

  // TODO 3 (FILLED)
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  
  // TODO 4 (FILLED)
  Wire.begin();

  byte error_eps, error_main, address;
  int eps_device_count = 0;
  int main_device_count = 0;
  bool foundTMP102 = false;
  bool foundRTC = false;

  Serial.println("Scanning both I2C Buses...");

  // TODO 5 (FILLED)
  for (address = 1; address < 128; address++) {
    I2C_EPS.beginTransmission(address);
    error_eps = I2C_EPS.endTransmission();

    if (error_eps == 0) {
      Serial.print("Found device on [EPS Bus]  at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      eps_device_count++;
      if (address == 0x4A) foundTMP102 = true;
    }

    Wire.beginTransmission(address);
    error_main = Wire.endTransmission();

    if (error_main == 0) {
      Serial.print("Found device on [Main Bus] at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      main_device_count++;
      if (address == 0x51) foundRTC = true;
    }
  } 
}

void loop() {
  
}
  