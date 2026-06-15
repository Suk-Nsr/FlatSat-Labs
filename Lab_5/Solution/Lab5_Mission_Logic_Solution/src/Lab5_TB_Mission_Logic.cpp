#include "Lab5_TB_Mission_Logic.h"

void runMissionTestbench(bool inTargetArea) {
  int powerPinState = digitalRead(PD1);

  if (inTargetArea && powerPinState == HIGH) {
    Serial.println("[Testbench] ✅ Payload Power is correctly ON when inside ROI.");
  } else if (!inTargetArea && powerPinState == LOW) {
    Serial.println("[Testbench] ✅ Payload Power is correctly OFF (Power Save) when outside ROI.");
  } else {
    Serial.println("[Testbench] ❌ Power State mismatch! Check your Payload Power Pin logic.");
  }
}
