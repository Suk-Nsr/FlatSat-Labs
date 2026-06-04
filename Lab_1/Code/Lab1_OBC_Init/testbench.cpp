#include "testbench.h"

// ====================================================================
// 🛑 TESTBENCH SECTION - DO NOT MODIFY THIS CODE 🛑
// This function performs a robust verification of the setup process.
// ====================================================================
void runBootTestbench(String msg, int rxPin, int txPin) {
  Serial.println("\n=================================");
  Serial.println("    SYSTEM DIAGNOSTIC TEST       ");
  Serial.println("=================================");
  
  bool pinTestPassed = false;
  bool msgTestPassed = false;

  // --- TEST 1: UART Pins Configuration ---
  Serial.println("[TEST 1] Checking UART Pins...");
  if (rxPin == PD9 && txPin == PD8) {
    Serial.println("  ✅ [PASS] RX and TX pins are assigned correctly.");
    pinTestPassed = true;
  } else {
    Serial.println("  ❌ [FAIL] Incorrect RX/TX pins.");
    
    // Check for common pin assignment errors
    if (rxPin == PD8 && txPin == PD9) {
      Serial.println("     * Hint: You swapped RX and TX! (RX is Receive, TX is Transmit)");
    } else if (rxPin == 0 || txPin == 0) {
      Serial.println("     * Hint: You haven't changed the pins from '0'. Please read the docs!");
    } else {
      Serial.println("     * Hint: Check the FlatSat schematic or documentation again.");
    }
  }

  // --- TEST 2: Boot Message Formatting ---
  Serial.println("\n[TEST 2] Checking Boot Message...");
  String cleanMsg = msg;
  cleanMsg.trim(); // Prevent false negatives from trailing spaces/newlines

  if (cleanMsg == "OBC Initialized. System Booting...") {
    Serial.println("  ✅ [PASS] Boot message is perfectly formatted!");
    msgTestPassed = true;
  } else if (cleanMsg == "") {
    Serial.println("  ❌ [FAIL] Boot message is empty. Please complete TODO 3.");
  } else {
    Serial.println("  ⚠️ [FAIL] Message mismatch detected.");
    Serial.println("     -> Your Output: \"" + cleanMsg + "\"");
    Serial.println("     -> Expected:    \"OBC Initialized. System Booting...\"");
    
    // Check specific common syntax errors
    if (cleanMsg.indexOf("System Booting") == -1) {
      Serial.println("     * Hint: Did you forget to include 'System Booting...'?");
    }
    if (cleanMsg.indexOf("obc") != -1 || cleanMsg.indexOf("Obc") != -1) {
      Serial.println("     * Hint: Satellite identifiers like 'OBC' must be fully uppercase.");
    }
  }

  // --- FINAL RESULT ---
  Serial.println("\n--- Test Summary ---");
  if (pinTestPassed && msgTestPassed) {
    Serial.println("🌟 MISSION ACCOMPLISHED: OBC Initialization Complete! 🌟");
  } else {
    Serial.println("🔧 MISSION FAILED: Please fix the errors above and try again.");
  }
  Serial.println("=================================\n");
}