// NBSPACE Labs: FlatSat Learning Set
// Lab 5.2: GPS-Gating & Orbital Constraints
// Objective: Learn to evaluate GPS coordinates against a predefined Region of Interest (ROI)
// Skeleton Code

#include <TinyGPS++.h>
#include "src/Lab5_TB_GPS_Gating.h"

// Define the Region of Interest (ROI) - e.g., Thailand Bounding Box
const float ROI_LAT_MIN = 5.61;
const float ROI_LAT_MAX = 20.46;
const float ROI_LON_MIN = 97.34;
const float ROI_LON_MAX = 105.63;

TinyGPSPlus gps;
HardwareSerial gps_uart(PE0, PE1);

void setup() {
  Serial.setTx(PD8);
  Serial.setRx(PD9);
  Serial.begin(115200);

  gps_uart.begin(9600);

  Serial.println("Lab 5.2: GPS Gating Logic Started");
}

void loop() {
  // --- Section 1: Read GPS Coordinates ---
  // Read data from the GPS UART
  while (gps_uart.available() > 0) {
    gps.encode(gps_uart.read());
  }

  // Run logic every 2 seconds
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 2000) {
    lastCheck = millis();

    // Directly assign coordinates without checking for a valid fix
    float currentLat = gps.location.lat();
    float currentLon = gps.location.lng();

    bool inTargetArea = false;

    // --- Section 2: GPS-Gating Logic ---
    // TODO 1: Determine if currentLat and currentLon are within the ROI boundaries
    // Hint: Use an if-else condition checking against ROI_LAT_MIN, ROI_LAT_MAX, ROI_LON_MIN, and ROI_LON_MAX.
    // Update the inTargetArea boolean variable accordingly.
    
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
  }
}
