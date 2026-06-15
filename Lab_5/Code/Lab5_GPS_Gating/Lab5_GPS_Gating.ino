// NBSPACE Labs: FlatSat Learning Set
// Lab 5.2: GPS-Gating & Orbital Constraints
// Objective: Learn to evaluate GPS coordinates against a predefined Region of Interest (ROI)

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
  // --- Section 1: Simulated GPS Coordinates ---
  // In a real scenario, these would come from the GPS module.
  // For this lab, we will simulate the coordinates to test the logic.
  
  float currentLat = 13.7563; // Example: Bangkok Latitude
  float currentLon = 100.5018; // Example: Bangkok Longitude
  
  // Try changing these values to test outside the ROI:
  // float currentLat = 35.6895; // Example: Tokyo Latitude
  // float currentLon = 139.6917; // Example: Tokyo Longitude

  bool inTargetArea = false;

  // --- Section 2: GPS-Gating Logic ---
  // TODO 1: Write an if-else condition to check if currentLat and currentLon 
  // are within the ROI_LAT_MIN, ROI_LAT_MAX and ROI_LON_MIN, ROI_LON_MAX.
  // If they are inside the area, set inTargetArea to true. Otherwise, false.
  
  // [Add your code here]
  

  // Print the result
  Serial.print("Current Location: ");
  Serial.print(currentLat, 4); Serial.print(", "); Serial.println(currentLon, 4);
  
  if (inTargetArea) {
    Serial.println("Status: INSIDE TARGET AREA");
    // Action: Payload would be triggered here
  } else {
    Serial.println("Status: OUTSIDE TARGET AREA");
    // Action: System goes into Standby/Power Saving here
  }

  runGPSGatingTestbench(currentLat, currentLon, inTargetArea);
  
  delay(2000); // Wait before next check
}
