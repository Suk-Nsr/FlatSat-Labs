// NBSPACE Labs: FlatSat Learning Set
// Lab 5.3: Mission Logic & Payload Control
// Solution Code

#include <Wire.h>
#include <PCF85063TP.h>
#include <TinyGPS++.h>
#include <SPI.h>
#include "SdFat.h"
#include "Arducam_Mega.h"
#include "src/Lab5_TB_Mission_Logic.h"

// Define ROI (Region of Interest)
const float ROI_LAT_MIN = 5.61;
const float ROI_LAT_MAX = 20.46;
const float ROI_LON_MIN = 97.34;
const float ROI_LON_MAX = 105.63;

// Initialize Hardware interfaces
HardwareSerial gps_uart(PE0, PE1);
PCD85063TP rtc;
TinyGPSPlus gps;

// SD Card and Camera definitions
#define SPI_DRIVER_SELECT 2
#define ENABLE_DEDICATED_SPI 1

SPIClass SD_SPI(PC12, PC11, PC10);
const int SD_CS = PC9;
#define SD_CONFIG SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(8), &SD_SPI)
SdFs sd;
FsFile file;

const int CAM_CS = PE_7;
Arducam_Mega myCAM(CAM_CS);
#define BUFFER_SIZE 0xff

// Power Control Pin for Payload
const int PAYLOAD_POWER_PIN = PD1;

void setup() {
  Serial.setTx(PD8);
  Serial.setRx(PD9);
  Serial.begin(115200);

  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();
  
  gps_uart.begin(9600);
  rtc.begin();

  // Set the PAYLOAD_POWER_PIN as OUTPUT
  pinMode(PAYLOAD_POWER_PIN, OUTPUT);
  digitalWrite(PAYLOAD_POWER_PIN, LOW); // Start with power off

  // Initialize Camera SPI
  SPI.setMISO(PB_4);
  SPI.setMOSI(PB_5);
  SPI.setSCLK(PB_3);
  SPI.begin();

  Serial.println("Lab 5.3: Mission Logic Started");

  // Initialize SD Card
  if (!sd.begin(SD_CONFIG)) {
    Serial.println("SD Card initialization failed!");
  } else {
    Serial.println("SD Card initialized.");
  }
}

void loop() {
  // Read GPS Data
  while (gps_uart.available() > 0) {
    gps.encode(gps_uart.read());
  }

  // Run Mission Logic every 5 seconds
  static unsigned long lastMissionRun = 0;
  if (millis() - lastMissionRun > 5000) {
    lastMissionRun = millis();

    if (!gps.location.isValid()) {
      Serial.println("Waiting for GPS fix...");
      return;
    }

    float currentLat = gps.location.lat();
    float currentLon = gps.location.lng();
    
    Serial.print("Current Location: ");
    Serial.print(currentLat, 4); Serial.print(", "); Serial.println(currentLon, 4);

    bool inTargetArea = false;

    // GPS-Gating logic
    if (currentLat >= ROI_LAT_MIN && currentLat <= ROI_LAT_MAX &&
        currentLon >= ROI_LON_MIN && currentLon <= ROI_LON_MAX) {
      inTargetArea = true;
    }

    if (inTargetArea) {
      Serial.println("Inside ROI. Executing Mission payload...");
      
      // Turn ON Payload power
      digitalWrite(PAYLOAD_POWER_PIN, HIGH);
      delay(500); // Give camera time to boot

      // Ensure Camera is initialized
      myCAM.begin();

      // Read RTC time for Metadata
      rtc.getTime();
      
      // --- CAPTURE & SAVE IMAGE ---
      myCAM.takePicture(CAM_IMAGE_MODE_VGA, CAM_IMAGE_PIX_FMT_JPG);
      // (Image saving bytes logic would go here, omitted for brevity but students could copy from examples)
      
      // --- METADATA LOGGING ---
      if (file.open("METADATA.TXT", O_RDWR | O_CREAT | O_APPEND)) {
        file.print("Time: ");
        file.print(rtc.hour); file.print(":"); file.print(rtc.minute); file.print(":"); file.print(rtc.second);
        file.print(" | Location: ");
        file.print(currentLat, 6); file.print(", "); file.println(currentLon, 6);
        file.close();
        Serial.println("Metadata saved successfully.");
      } else {
        Serial.println("Failed to open METADATA.TXT");
      }

      Serial.println("Payload execution completed.");
      
    } else {
      Serial.println("Outside ROI. Entering Power Save Mode.");
      
      // Turn OFF Payload power
      digitalWrite(PAYLOAD_POWER_PIN, LOW);
    }

    runMissionTestbench(inTargetArea);
  }
}
