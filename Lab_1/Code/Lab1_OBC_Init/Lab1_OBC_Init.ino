
// NBSPACE Labs: FlatSat Learning Set
// Lab 1.1: OBC Initialization
// Objective: Learn how to initialize the OBC and I2C Subsystems


#include "testbench.h"

// TODO 1: Find the UART pins for RX and TX from the FlatSat OBC Documentation.
// Replace the placeholder values with the correct pin names (e.g., PA10, PB6, etc.).
const int OBC_RX_PIN = PD9; 
const int OBC_TX_PIN = PD8;

String bootMessage = "";

void setup() {
  Serial.setRx(OBC_RX_PIN);
  Serial.setTx(OBC_TX_PIN);

  // TODO 2: Initialize the Serial Monitor with a Baud Rate of 115200.
  // [Add your code here]
  
  while (!Serial) {;}

  // TODO 3: Assign the exact text "OBC Initialized. System Booting..." to bootMessage
  // [Add your code here]

  // TODO 4: Print the bootMessage to the Serial Monitor
  // [Add your code here]

  runBootTestbench(bootMessage, OBC_RX_PIN, OBC_TX_PIN);
}