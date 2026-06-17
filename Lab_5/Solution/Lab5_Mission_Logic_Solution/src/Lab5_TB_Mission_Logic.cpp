#include "Lab5_TB_Mission_Logic.h"

void runMissionTestbench(float lat, float lon, bool studentInTargetArea) {
  const float ROI_LAT_MIN = 5.61;
  const float ROI_LAT_MAX = 20.46;
  const float ROI_LON_MIN = 97.34;
  const float ROI_LON_MAX = 105.63;
  
  bool expectedInTargetArea = (lat >= ROI_LAT_MIN && lat <= ROI_LAT_MAX && lon >= ROI_LON_MIN && lon <= ROI_LON_MAX);
  
  // Check for Power pin state
  int powerPinState = digitalRead(PD1); // PAYLOAD_POWER_PIN

  bool logicCorrect = (studentInTargetArea == expectedInTargetArea);
  bool pinCorrect = false;

  if (expectedInTargetArea && powerPinState == HIGH) {
    pinCorrect = true;
  } else if (!expectedInTargetArea && powerPinState == LOW) {
    pinCorrect = true;
  }

  if (logicCorrect && pinCorrect) {
    Serial.println("[Testbench] ✅ Mission Logic & Payload Power are CORRECT.");
  } else {
    if (!logicCorrect) {
      Serial.println("[Testbench] ❌ Gating logic is INCORRECT. Check ROI conditions.");
    }
    if (!pinCorrect) {
      Serial.println("[Testbench] ❌ Power State mismatch! Check PAYLOAD_POWER_PIN logic.");
    }
  }
}
