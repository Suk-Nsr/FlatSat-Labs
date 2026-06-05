/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 1.2: Dual I2C Scanner (Template with Robust Testbench)
 * Objective: Scan for I2C Addresses on BOTH subsystem buses (EPS and Main).
 */

#include <Wire.h>

TwoWire I2C_EPS(PF0, PF1); // EPS Bus object

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  
  while (!Serial){;}

  Serial.println("\n--- FlatSat Dual I2C Scanner ---");

  // TODO 1: Initialize the EPS I2C bus (I2C_EPS)
  // [Add your code here]
  

  // Override default Wire pins for Main/RTC Bus
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  
  // TODO 2: Initialize the Main I2C bus (Wire)
  // [Add your code here]
  
}

void loop() {
  byte error_eps, error_main, address;
  int eps_device_count = 0;
  int main_device_count = 0;
  bool foundTMP102 = false;
  bool foundRTC = false;

  Serial.println("Scanning both I2C Buses...");

  // TODO 3: Create a loop to scan addresses from 1 to 127
  // [Write your 'for' loop condition below]
  
  
    // --- INSIDE THE LOOP ---
    // 1. Scan the EPS Bus
    I2C_EPS.beginTransmission(address);
    error_eps = I2C_EPS.endTransmission();

    if (error_eps == 0) {
      Serial.print("Found device on [EPS Bus]  at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      eps_device_count++;
      
      // Hardware-level tracking for Testbench
      if (address == 0x4A) foundTMP102 = true;
    }

    // 2. Scan the Main/RTC Bus
    Wire.beginTransmission(address);
    error_main = Wire.endTransmission();

    if (error_main == 0) {
      Serial.print("Found device on [Main Bus] at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      main_device_count++;
      
      // Hardware-level tracking for Testbench
      if (address == 0x51) foundRTC = true;
    }
  // -----------------------
  // [Don't forget to close the loop bracket here!]


  // Run the Built-In Self-Test (BIST)
  runScannerTestbench(eps_device_count, main_device_count, foundTMP102, foundRTC);

  delay(5000); 
}

// ====================================================================
// 🛑 TESTBENCH SECTION - DO NOT MODIFY THIS CODE 🛑
// This function verifies I2C bus routing and vital peripheral existence.
// ====================================================================
void runScannerTestbench(int eps_count, int main_count, bool hasTMP, bool hasRTC) {
  Serial.println("\n--- Running BIST (I2C Subsystem Health Check) ---");
  bool healthy = true;

  if (eps_count == 0 && main_count == 0) {
    Serial.println("❌ [CRITICAL] I2C buses completely inactive. Verify TODO 1 and TODO 2!");
    healthy = false;
  } else {
    if (hasTMP) {
      Serial.println("✅ [OK] EPS Bus: TMP102 Sensor detected at 0x4A.");
    } else {
      Serial.println("❌ [FAIL] EPS Bus: Missing vital TMP102 at 0x4A. Check PF0/PF1 assignment.");
      healthy = false;
    }

    if (hasRTC) {
      Serial.println("✅ [OK] Main Bus: PCF85063TP RTC detected at 0x51.");
    } else {
      Serial.println("❌ [FAIL] Main Bus: Missing vital RTC at 0x51. Check PB8/PB9 assignment.");
      healthy = false;
    }
  }

  if (healthy) {
    Serial.println("🌟 [STATUS] PASS: I2C hardware fabric integrated successfully!");
  } else {
    Serial.println("⚠️ [STATUS] FAIL: Hardware fabric integration errors detected.");
  }
  Serial.println("---------------------------------------------------\n");
}