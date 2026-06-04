/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 1.1: OBC Initialization (Template with Testbench)
 * Objective: Learn how to initialize the OBC and use the Serial Monitor.
 */

String bootMessage = ""; // Variable to store your boot message

void setup() {
  // Hardware-specific UART pins for FlatSat OBC
  Serial.setRx(PD9);
  Serial.setTx(PD8);

  // TODO 1: Initialize the Serial Monitor with a Baud Rate of 115200.
  // [Add your code here]
  

  delay(2000); // Wait for the Serial port to be ready

  // TODO 2: Assign the exact text "OBC Initialized. System Booting..." to bootMessage
  // [Add your code here]
  

  // TODO 3: Print the bootMessage to the Serial Monitor
  // [Add your code here]
  

  // Run the Built-In Self-Test (BIST)
  runBootTestbench(bootMessage);
}

void loop() {
  // Nothing to do here yet.
}

// ====================================================================
// 🛑 TESTBENCH SECTION - DO NOT MODIFY THIS CODE 🛑
// This function performs a robust verification of the boot message string.
// ====================================================================
void runBootTestbench(String msg) {
  Serial.println("\n--- Running BIST (Boot Test) ---");
  
  String cleanMsg = msg;
  cleanMsg.trim(); // Prevent false negatives from trailing spaces/newlines

  if (cleanMsg == "OBC Initialized. System Booting...") {
    Serial.println("✅ [PASS] Boot message is perfectly formatted!");
  } else if (cleanMsg == "") {
    Serial.println("❌ [FAIL] Boot message is empty. Please complete TODO 2.");
  } else {
    Serial.println("⚠️ [FAIL] Message mismatch detected.");
    Serial.println("   -> Your Output: \"" + cleanMsg + "\"");
    Serial.println("   -> Expected:    \"OBC Initialized. System Booting...\"");
    
    // Check specific common errors
    if (cleanMsg.indexOf("System Booting") == -1) {
      Serial.println("   * Hint: Did you forget to include 'System Booting...'?");
    }
    if (cleanMsg.indexOf("obc") != -1 || cleanMsg.indexOf("Obc") != -1) {
      Serial.println("   * Hint: Satellite identifiers like 'OBC' must be fully uppercase.");
    }
  }
  Serial.println("--------------------------------\n");
}