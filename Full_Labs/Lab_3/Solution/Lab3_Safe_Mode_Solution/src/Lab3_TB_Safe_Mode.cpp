#include "Lab3_TB_Safe_Mode.h"

void runSafeModeTestbench(float voltage, float temp, bool triggered, float threshold) {
  Serial.println("\n========================================");
  Serial.println("    VALIDATION TEST       ");
  Serial.println("========================================");

  bool isPassed = true;

  // Test 1: Voltage reading
  Serial.println("\n[TEST 1] Checking Voltage Reading...");
  if (voltage == 0.0) {
    Serial.println(" 🔴 [FAIL] Voltage reading is 0.00V. Sensor not responding.");
    isPassed = false;
  } else if (voltage < 0.0 || voltage > 26.0) {
    Serial.println(" 🔴 [FAIL] Voltage " + String(voltage, 2) + "V is outside expected range.");
    isPassed = false;
  } else {
    Serial.println(" 🟢 [PASS] Voltage: " + String(voltage, 2) + "V reading captured.");
  }

  // Test 2: Temperature reading
  Serial.println("\n[TEST 2] Checking Temperature Reading...");
  if (temp == 0.0) {
    Serial.println(" 🔴 [FAIL] Temperature reading is 0.00C. Sensor not responding.");
    isPassed = false;
  } else if (temp < -40.0 || temp > 125.0) {
    Serial.println(" 🔴 [FAIL] Temperature " + String(temp, 2) + "C is outside TMP102 range.");
    isPassed = false;
  } else {
    Serial.println(" 🟢 [PASS] Temperature: " + String(temp, 2) + "C reading captured.");
  }

  // Test 3: Safe mode logic validation
  Serial.println("\n[TEST 3] Checking Safe Mode Logic...");
  bool expectedTrigger = (voltage > 0.0 && voltage < threshold);
  
  if (expectedTrigger && !triggered) {
    Serial.println(" 🔴 [FAIL] Voltage is below threshold but safe mode was NOT triggered!");
    Serial.println("     * Hint: Complete TODO 3 — compare busVoltage against SAFE_MODE_THRESHOLD.");
    isPassed = false;
  } else if (!expectedTrigger && triggered) {
    Serial.println(" 🔴 [FAIL] Safe mode triggered when voltage is nominal!");
    isPassed = false;
  } else if (triggered) {
    Serial.println(" 🟢 [PASS] Safe Mode correctly triggered at " + String(voltage, 2) + "V.");
  } else {
    Serial.println(" 🟢 [PASS] Safe Mode not triggered (voltage is nominal).");
  }

  // Test 4: Hardware Power Pin State
  Serial.println("\n[TEST 4] Checking Hardware Power Pin States...");
  int commsState = digitalRead(PD1);
  int payload1State = digitalRead(PD2);
  int payload2State = digitalRead(PD3);

  if (expectedTrigger) {
    if (commsState == LOW && payload1State == LOW && payload2State == LOW) {
      Serial.println(" 🟢 [PASS] All non-essential loads are DISABLED (LOW) during Safe Mode.");
    } else {
      Serial.println(" 🔴 [FAIL] Loads are still active! Ensure shutdownAllChannels() writes LOW to PD1, PD2, and PD3.");
      isPassed = false;
    }
  } else {
    if (commsState == HIGH && payload1State == HIGH && payload2State == HIGH) {
      Serial.println(" 🟢 [PASS] All payloads are ACTIVE (HIGH) during nominal operation.");
    } else {
      Serial.println(" 🔴 [FAIL] Payloads are disabled but voltage is nominal! Check initialization.");
      isPassed = false;
    }
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
