#include "Lab1_TB_OBC_Init.h"

void runBootTestbench(String msg, int rxPin, int txPin) {
  Serial.println("\n========================================");
  Serial.println("    VALIDATION TEST       ");
  Serial.println("========================================");
  
  bool msgTestPassed = false;

  Serial.println("\n[TEST 1] Checking Boot Message...");
  String cleanMsg = msg;
  cleanMsg.trim();

  if (cleanMsg == "OBC Initialized. System Booting...") {
    Serial.println(" 🟢 [PASS] Boot message is perfectly formatted!");
    msgTestPassed = true;
  } else if (cleanMsg == "") {
    Serial.println(" 🔴 [FAIL] Boot message is empty. Please complete TODO 3.");
  } else {
    Serial.println(" 🔴 [FAIL] Message mismatch detected.");
    Serial.println("     -> Your Output: \"" + cleanMsg + "\"");
    Serial.println("     -> Expected:    \"OBC Initialized. System Booting...\"");
    
    if (cleanMsg.indexOf("System Booting") == -1) {
      Serial.println("     * Hint: Did you forget to include 'System Booting...'?");
    }
    if (cleanMsg.indexOf("obc") != -1 || cleanMsg.indexOf("Obc") != -1) {
      Serial.println("     * Hint: Satellite identifiers like 'OBC' must be fully uppercase.");
    }
  }

  Serial.println("\n========================================\n--- Test Summary ---");
  if (msgTestPassed) {
    Serial.println(" 🟢 VALIDATE PASS: OBC Initialization Complete!");
  } else {
    Serial.println(" 🔴 VALIDATE FAILED: Please fix the errors and try again.");
  }
  Serial.println("========================================\n");
}