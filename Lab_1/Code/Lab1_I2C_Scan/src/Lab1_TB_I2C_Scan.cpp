#include "Lab1_TB_I2C_Scan.h"

void runI2CScanTestbench(int eps_device_count, int main_device_count, bool foundTMP102, bool foundRTC) {
  Serial.println("\n========================================");
  Serial.println("    VALIDATION TEST       ");
  Serial.println("========================================");
  
  bool epsTestPassed = false;
  bool mainTestPassed = false;

  Serial.println("\n[TEST 1] Checking EPS Bus Devices...");
  if (eps_device_count > 0 && foundTMP102) {
    Serial.println(" 🟢 [PASS] Found devices on EPS Bus, including TMP102 (0x4A).");
    epsTestPassed = true;
  } else {
    Serial.println(" 🔴 [FAIL] Missing expected devices on EPS Bus.");
    if (!foundTMP102) Serial.println("     * Hint: TMP102 (0x4A) was not found. Check EPS I2C pins (PF0, PF1) and initialization.");
  }

  Serial.println("\n[TEST 2] Checking Main Bus Devices...");
  if (main_device_count > 0 && foundRTC) {
    Serial.println(" 🟢 [PASS] Found devices on Main Bus, including RTC (0x51).");
    mainTestPassed = true;
  } else {
    Serial.println(" 🔴 [FAIL] Missing expected devices on Main Bus.");
    if (!foundRTC) Serial.println("     * Hint: RTC (0x51) was not found. Check Main I2C pins (PB9, PB8) and initialization.");
  }

  Serial.println("\n========================================\n--- Test Summary ---");
  if (epsTestPassed && mainTestPassed) {
    Serial.println(" 🟢 VALIDATE PASS: Both I2C Buses are configured correctly!");
  } else {
    Serial.println(" 🔴 VALIDATE FAILED: Please fix the errors and try again.");
  }
  Serial.println("========================================\n");
}
