
// NBSPACE Labs: FlatSat Learning Set
// Lab 3.2: OBC Power Control (Power Lines Manipulation)
// Solution Code


#include <Arduino.h>

#define PIN_COMMS      PD1
#define PIN_PAYLOAD_1  PD2
#define PIN_PAYLOAD_2  PD3 

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
  while (!Serial) {;}

  Serial.println("\n=== FlatSat Lab 3.2: Manual Power Control Booting ===");

  pinMode(PIN_COMMS, OUTPUT);
  pinMode(PIN_PAYLOAD_1, OUTPUT);
  pinMode(PIN_PAYLOAD_2, OUTPUT);

  digitalWrite(PIN_PAYLOAD_1, LOW);
  digitalWrite(PIN_PAYLOAD_2, LOW);
  digitalWrite(PIN_COMMS, LOW);
  Serial.println("Initial State: All Subsystems OFF (0V)");
  Serial.println("Waiting 3 seconds to begin sequence...\n");
  delay(3000);
}

void loop() {
  Serial.println("--- Power Sequence Start ---");

  digitalWrite(PIN_PAYLOAD_1, HIGH);
  Serial.println("Payload 1 (PD1) -> ON");
  delay(4000);

  digitalWrite(PIN_PAYLOAD_2, HIGH);
  Serial.println("Payload 2 (PD3) -> ON");
  delay(4000);

  digitalWrite(PIN_COMMS, HIGH);
  Serial.println("COMMS (PD2)     -> ON");
  delay(4000);

  digitalWrite(PIN_PAYLOAD_1, LOW);
  digitalWrite(PIN_PAYLOAD_2, LOW);
  digitalWrite(PIN_COMMS, LOW);
  Serial.println("All Subsystems  -> OFF");
  Serial.println("----------------------------\n");
  
  delay(5000);
}