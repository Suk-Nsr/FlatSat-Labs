#include "Lab3_TB_Safe_Mode.h"

void runSafeModeTestbench(float voltage, float temp, bool triggered, float threshold) {
  Serial.println("\n========================================");
  Serial.println("    VALIDATION TEST       ");
  Serial.println("========================================");

  bool isPassed = true;

  // Test 1: Threshold configured
  Serial.println("\n[TEST 1] Checking Safe Mode Threshold...");
  if (threshold == 0.0) {
    Serial.println(" 🔴 [FAIL] Threshold is still set to 0.0V (default).");
    Serial.println("     * Hint: Set SAFE_MODE_THRESHOLD in TODO 1 (e.g., 3.3).");
    isPassed = false;
  } else {
    Serial.println(" 🟢 [PASS] Threshold configured at " + String(threshold, 1) + "V.");
  }

  // Test 2: Voltage reading (Pre-filled)
  Serial.println("\n[TEST 2] Checking Voltage Reading...");
  if (voltage == 0.0) {
    Serial.println(" 🔴 [FAIL] Voltage reading is 0.00V. Sensor not responding.");
    isPassed = false;
  } else if (voltage < 0.0 || voltage > 26.0) {
    Serial.println(" 🔴 [FAIL] Voltage " + String(voltage, 2) + "V is outside expected range.");
    isPassed = false;
  } else {
    Serial.println(" 🟢 [PASS] Voltage: " + String(voltage, 2) + "V reading captured.");
  }

  // Test 3: Temperature reading (Pre-filled)
  Serial.println("\n[TEST 3] Checking Temperature Reading...");
  if (temp == 0.0) {
    Serial.println(" 🔴 [FAIL] Temperature reading is 0.00C. Sensor not responding.");
    isPassed = false;
  } else if (temp < -40.0 || temp > 125.0) {
    Serial.println(" 🔴 [FAIL] Temperature " + String(temp, 2) + "C is outside TMP102 range.");
    isPassed = false;
  } else {
    Serial.println(" 🟢 [PASS] Temperature: " + String(temp, 2) + "C reading captured.");
  }

  // Test 4: Safe mode logic validation
  Serial.println("\n[TEST 4] Checking Safe Mode Logic...");
  if (voltage > 0.0 && voltage < threshold && !triggered) {
    Serial.println(" 🔴 [FAIL] Voltage is below threshold but safe mode was NOT triggered!");
    Serial.println("     * Hint: Complete TODO 2 — compare busVoltage against SAFE_MODE_THRESHOLD.");
    isPassed = false;
  } else if (triggered) {
    Serial.println(" 🟢 [PASS] Safe Mode correctly triggered at " + String(voltage, 2) + "V.");
  } else {
    Serial.println(" 🟢 [PASS] Safe Mode not triggered (voltage is nominal).");
  }

  Serial.println("\n========================================");
  Serial.println("--- Test Summary ---");
  if (isPassed) {
    Serial.println(" 🟢 VALIDATE PASS: Safe Mode Controller Verified!");
  } else {
    Serial.println(" 🔴 VALIDATE FAILED: Please fix the errors and try again.");
  }
  Serial.println("========================================\n");
}
