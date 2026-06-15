// NBSPACE Labs: FlatSat Learning Set
// Lab 5.2: GPS-Gating & Orbital Constraints
// Solution Code

#include <TinyGPS++.h>
#include "src/Lab5_TB_GPS_Gating.h"

// Define the Region of Interest (ROI) - e.g., Thailand Bounding Box
const float ROI_LAT_MIN = 5.61;
const float ROI_LAT_MAX = 20.46;
const float ROI_LON_MIN = 97.34;
const float ROI_LON_MAX = 105.63;

TinyGPSPlus gps;

void setup() {
  Serial.setTx(PD8);
  Serial.setRx(PD9);
  Serial.begin(115200);

  Serial.println("Lab 5.2: GPS Gating Logic Started");
}

void loop() {
  // Simulated GPS Coordinates
  float currentLat = 13.7563; // Example: Bangkok Latitude
  float currentLon = 100.5018; // Example: Bangkok Longitude
  
  bool inTargetArea = false;

  // --- Section 2: GPS-Gating Logic ---
  if (currentLat >= ROI_LAT_MIN && currentLat <= ROI_LAT_MAX &&
      currentLon >= ROI_LON_MIN && currentLon <= ROI_LON_MAX) {
    inTargetArea = true;
  } else {
    inTargetArea = false;
  }

  // Print the result
  Serial.print("Current Location: ");
  Serial.print(currentLat, 4); Serial.print(", "); Serial.println(currentLon, 4);
  
  if (inTargetArea) {
    Serial.println("Status: INSIDE TARGET AREA");
  } else {
    Serial.println("Status: OUTSIDE TARGET AREA");
  }

  runGPSGatingTestbench(currentLat, currentLon, inTargetArea);
  
  delay(2000); // Wait before next check
}
