
// NBSPACE Labs: FlatSat Learning Set
// Lab 2.5: OBC Power Control (Interactive Serial Control)
// Solution Code

#include <Arduino.h>

// TODO 1: Assign the correct hardware pins for each subsystem
#define PIN_COMMS      ???
#define PIN_PAYLOAD_1  ???
#define PIN_PAYLOAD_2  ??? 

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);

  delay(4000);

  Serial.println("\n=== FlatSat Lab 2.5: Interactive Power Control ===");
  Serial.println("Type the following keys in the Serial Monitor and press Enter:");
  Serial.println("  '1' = Payload 1 ON   |  'q' = Payload 1 OFF");
  Serial.println("  '2' = Payload 2 ON   |  'w' = Payload 2 OFF");
  Serial.println("  '3' = COMMS ON       |  'e' = COMMS OFF");
  Serial.println("  '0' = Turn ALL OFF");
  Serial.println("--------------------------------------------------");

  // TODO 2: Configure the subsystem pins as OUTPUT
  // [Add your code here]

  digitalWrite(PIN_PAYLOAD_1, HIGH);
  digitalWrite(PIN_PAYLOAD_2, HIGH);
  digitalWrite(PIN_COMMS, HIGH);
  Serial.println("Initial State: All Subsystems ON\n");
}

void loop() {
  // Check if any data has been typed into the Serial Monitor
  if (Serial.available() > 0) {
    char cmd = Serial.read(); // Read the first character

    // Match the character to the corresponding command
    switch (cmd) {
      // --- Payload 1 (PD2) ---
      // TODO 3: Implement the logic to turn Payload 1 ON and OFF
      case '1':
        // [Add your code here]
        Serial.println("Payload 1 (PD2) -> ON");
        break;
      case 'q':
        // [Add your code here]
        Serial.println("Payload 1 (PD2) -> OFF");
        break;

      // --- Payload 2 (PD3) ---
      // TODO 4: Implement the logic to turn Payload 2 ON and OFF
      case '2':
        // [Add your code here]
        Serial.println("Payload 2 (PD3) -> ON");
        break;
      case 'w':
        // [Add your code here]
        Serial.println("Payload 2 (PD3) -> OFF");
        break;

      // --- COMMS (PD1) ---
      // TODO 5: Implement the logic to turn the COMMS module ON and OFF
      case '3':
        // [Add your code here]
        Serial.println("COMMS (PD1)     -> ON");
        break;
      case 'e':
        // [Add your code here]
        Serial.println("COMMS (PD1)     -> OFF");
        break;

      // --- ALL OFF ---
      case '0':
        // TODO 6: Turn off all subsystems
        // [Add your code here]
        Serial.println("All Subsystems  -> OFF");
        break;

      // Ignore standard invisible line-ending characters
      case '\n':
      case '\r':
      case ' ':
        break;

      // Handle unrecognized inputs
      default:
        Serial.println("Unknown command. Use 1/q, 2/w, 3/e, or 0.");
        break;
    }
  }
}