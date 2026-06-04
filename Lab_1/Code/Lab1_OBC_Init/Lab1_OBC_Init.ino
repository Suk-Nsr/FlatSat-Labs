/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 1.1: OBC Initialization (Template with Testbench)
 * Objective: Learn how to initialize the OBC and use the Serial Monitor.
 */

#include "testbench.h" // Includes your separated testbench file

// TODO 1: ค้นหาขา UART สำหรับ RX และ TX จากเอกสาร (Documentation) ของ FlatSat OBC
// แทนที่ค่า '0' ด้วยชื่อพินที่ถูกต้อง (เช่น PA10, PB6, ฯลฯ)
const int OBC_RX_PIN = PD9; 
const int OBC_TX_PIN = PD8;

String bootMessage = ""; // Variable to store your boot message

void setup() {
  // นำตัวแปรพินที่ตั้งค่าไว้มาใช้งาน
  Serial.setRx(OBC_RX_PIN);
  Serial.setTx(OBC_TX_PIN);

  // TODO 2: Initialize the Serial Monitor with a Baud Rate of 115200.
  // [Add your code here]
  Serial.begin(115200);
  
  // Wait for the serial port to connect. This ensures you don't miss the initial messages!
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

  // TODO 3: Assign the exact text "OBC Initialized. System Booting..." to bootMessage
  // [Add your code here]
  bootMessage = "OBC Initialized. System Booting...";

  // TODO 4: Print the bootMessage to the Serial Monitor
  // [Add your code here]
  Serial.println(bootMessage);

  // Run the Built-In Self-Test (BIST)
  runBootTestbench(bootMessage, OBC_RX_PIN, OBC_TX_PIN);
}

void loop() {
  // Nothing to do here yet.
}