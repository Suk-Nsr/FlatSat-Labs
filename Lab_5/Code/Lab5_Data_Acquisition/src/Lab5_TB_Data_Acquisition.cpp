#include "Lab5_TB_Data_Acquisition.h"

void runDataAcquisitionTestbench() {
  static unsigned long lastRun = 0;
  if (millis() - lastRun > 5000) {
    lastRun = millis();
    // A simple testbench that just prints a verification message
    // In a real hardware testbench, we might verify I2C/UART states here.
    Serial.println("[Testbench] Data Acquisition check running...");
  }
}
