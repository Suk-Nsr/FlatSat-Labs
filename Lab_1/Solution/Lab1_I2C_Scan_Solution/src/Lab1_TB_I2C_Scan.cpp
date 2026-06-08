#include "Lab1_TB_I2C_Scan.h"

void runI2CScanTestbench(bool eps_found[128], bool main_found[128]) {
  Serial.println("\n========================================");
  Serial.println("    VALIDATION TEST       ");
  Serial.println("========================================");
  
  bool testPassed = true;

  const byte expected_eps[] = {0x40, 0x41, 0x42, 0x43, 0x47, 0x48, 0x4A, 0x4B, 0x58, 0x59, 0x5A, 0x5B};
  const byte expected_main[] = {0x49, 0x51};

  Serial.println("\n[TEST 1] Checking All I2C Devices...");
  
  for (int i = 0; i < sizeof(expected_eps); i++) {
    if (!eps_found[expected_eps[i]]) {
      Serial.print(" 🔴 [FAIL] Missing expected device on EPS Bus at address 0x");
      if (expected_eps[i] < 16) Serial.print("0");
      Serial.println(expected_eps[i], HEX);
      testPassed = false;
    }
  }

  for (int i = 0; i < sizeof(expected_main); i++) {
    if (!main_found[expected_main[i]]) {
      Serial.print(" 🔴 [FAIL] Missing expected device on Main Bus at address 0x");
      if (expected_main[i] < 16) Serial.print("0");
      Serial.println(expected_main[i], HEX);
      testPassed = false;
    }
  }

  if (testPassed) {
    Serial.println(" 🟢 [PASS] Found all expected devices on EPS and Main buses.");
  }

  Serial.println("\n========================================\n--- Test Summary ---");
  if (testPassed) {
    Serial.println(" 🟢 VALIDATE PASS: Both I2C Buses are configured correctly!");
  } else {
    Serial.println(" 🔴 VALIDATE FAILED: Please fix the errors and try again.");
  }
  Serial.println("========================================\n");
}
