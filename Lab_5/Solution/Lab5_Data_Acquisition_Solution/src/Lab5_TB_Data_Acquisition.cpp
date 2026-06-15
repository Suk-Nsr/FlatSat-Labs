#include "Lab5_TB_Data_Acquisition.h"

void runDataAcquisitionTestbench() {
  static unsigned long lastRun = 0;
  if (millis() - lastRun > 5000) {
    lastRun = millis();
    Serial.println("[Testbench] Data Acquisition check running...");
  }
}
