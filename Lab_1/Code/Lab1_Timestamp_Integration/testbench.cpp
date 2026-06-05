#include "testbench.h"

void runTelemetryTestbench(String packet, int h, int m, int s, float temp) {
  Serial.println("\n========================================");
  Serial.println("    VALIDATION TEST       ");
  Serial.println("========================================");
  
  bool isPassed = true;

  Serial.println("\n[TEST 1] Checking Telemetry Packet...");
  String cleanPacket = packet;
  cleanPacket.trim();

  if (cleanPacket == "") {
    Serial.println(" 🔴 [FAIL] Telemetry string buffer is empty. Please complete the TODO.");
    isPassed = false;
  } else {
    bool formatError = false;

    // Check starting bracket
    if (!cleanPacket.startsWith("[")) {
      Serial.println(" 🔴 [FAIL] Missing starting bracket '['.");
      formatError = true;
    }
    
    // Check bracket index for zero-padding validation
    int bracketClose = cleanPacket.indexOf("]");
    if (bracketClose != 9) {
      Serial.println(" 🔴 [FAIL] Timestamp length mismatch.");
      Serial.println("     * Hint: Timestamp must be exactly 8 characters long [HH:MM:SS]. Check your leading zeros logic!");
      formatError = true;
    }

    // Check string bridge
    if (cleanPacket.indexOf("] OBC Temperature: ") == -1) {
      Serial.println(" 🔴 [FAIL] Missing or mistyped string bridge.");
      Serial.println("     * Hint: Ensure it matches exactly '] OBC Temperature: ' (watch your spacing!).");
      formatError = true;
    }

    // Check termination unit
    if (!cleanPacket.endsWith("°C") && !cleanPacket.endsWith("C")) {
      Serial.println(" 🔴 [FAIL] Missing temperature unit.");
      Serial.println("     * Hint: Telemetry data must terminate with the unit '°C' or 'C'.");
      formatError = true;
    }

    // Display final breakdown of Test 1
    if (formatError) {
      Serial.println("     -> Your Output: \"" + cleanPacket + "\"");
      isPassed = false;
    } else {
      Serial.println(" 🟢 [PASS] Telemetry framing and data alignment verified!");
      Serial.println("     -> Valid Output: \"" + cleanPacket + "\"");
    }
  }

  Serial.println("\n========================================");
  Serial.println("--- Test Summary ---");
  if (isPassed) {
    Serial.println(" 🟢 VALIDATE PASS: Telemetry Integration Complete!");
  } else {
    Serial.println(" 🔴 VALIDATE FAILED: Please fix the formatting errors and try again.");
  }
  Serial.println("========================================\n");
}