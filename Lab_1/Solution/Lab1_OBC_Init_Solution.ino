/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 1.1: OBC Initialization (Solution Key)
 */

String bootMessage = ""; 

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);

  // TODO 1 (FILLED)
  Serial.begin(115200);
  
  while (!Serial){;}

  // TODO 2 (FILLED)
  bootMessage = "OBC Initialized. System Booting...";

  // TODO 3 (FILLED)
  Serial.println(bootMessage);

  runBootTestbench(bootMessage);
}

void loop() {}

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