#include "testbench.h"

void runEPSTestbench(float temp, float voltage, uint8_t inaAddr) {
  Serial.println("\n========================================");
  Serial.println("    VALIDATION TEST       ");
  Serial.println("========================================");

  bool isPassed = true;

  // Test 1: INA219 Address
  Serial.println("\n[TEST 1] Checking INA219 Address...");
  if (inaAddr == 0x00) {
    Serial.println(" 🔴 [FAIL] INA219 address is still set to placeholder 0x00!");
    Serial.println("     * Hint: Run your Lab 1.2 I2C scanner to find the correct address on the EPS bus.");
    isPassed = false;
  } else {
    Serial.println(" 🟢 [PASS] INA219 address configured as 0x" + String(inaAddr, HEX) + ".");
  }

  // Test 2: Temperature Sensor (Pre-filled, should always pass)
  Serial.println("\n[TEST 2] Checking Temperature Reading...");
  if (temp == 0.0) {
    Serial.println(" 🔴 [FAIL] Temperature reading is 0.00C (TMP102 not responding).");
    isPassed = false;
  } else if (temp < -40.0 || temp > 125.0) {
    Serial.println(" 🔴 [FAIL] Temperature " + String(temp, 2) + "C is outside TMP102 range.");
    isPassed = false;
  } else {
    Serial.println(" 🟢 [PASS] Temperature: " + String(temp, 2) + "C — Sensor OK.");
  }

  // Test 3: Bus Voltage
  Serial.println("\n[TEST 3] Checking Bus Voltage Reading...");
  if (voltage == 0.0) {
    Serial.println(" 🔴 [FAIL] Bus Voltage is 0.00V (INA219 not responding).");
    Serial.println("     * Hint: Complete TODO 2 — follow the I2C register read steps in the comments.");
    isPassed = false;
  } else if (voltage < 0.0 || voltage > 26.0) {
    Serial.println(" 🔴 [FAIL] Bus Voltage " + String(voltage, 2) + "V is outside expected range (0-26V).");
    Serial.println("     * Hint: Remember to shift right by 3 bits before multiplying by 0.004.");
    isPassed = false;
  } else {
    Serial.println(" 🟢 [PASS] Bus Voltage: " + String(voltage, 2) + "V — Power rail reading valid.");
  }

  Serial.println("\n========================================");
  Serial.println("--- Test Summary ---");
  if (isPassed) {
    Serial.println(" 🟢 VALIDATE PASS: EPS Telemetry Pipeline Operational!");
  } else {
    Serial.println(" 🔴 VALIDATE FAILED: Please fix the errors and try again.");
  }
  Serial.println("========================================\n");
}
