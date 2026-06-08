/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 1.3: Timestamp Integration (Solution Key)
 */

#include <Wire.h>
#include <PCF85063TP.h> 

TwoWire I2C_EPS(PF0, PF1);
PCD85063TP rtc; 
#define TMP102_ADDRESS 0x4A 

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);
 
  while (!Serial){;} 
  
  I2C_EPS.begin();
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin(); 
  rtc.begin();
  
  Serial.println("OBC Telemetry System Started...");
}

void loop() {
  rtc.getTime(); 
  int h = rtc.hour;
  int m = rtc.minute;
  int s = rtc.second;

  float boardTemp = 0.0;
  I2C_EPS.requestFrom(TMP102_ADDRESS, 2); 
  if (I2C_EPS.available() == 2) {
    byte msb = I2C_EPS.read();
    byte lsb = I2C_EPS.read();
    int tempRaw = ((msb << 8) | lsb) >> 4;
    boardTemp = tempRaw * 0.0625;
  }

  // TODO (FILLED): Build compiled telemetry string packet
  String telemetryPacket = "[";
  
  if (h < 10) telemetryPacket += "0";
  telemetryPacket += String(h) + ":";
  
  if (m < 10) telemetryPacket += "0";
  telemetryPacket += String(m) + ":";
  
  if (s < 10) telemetryPacket += "0";
  telemetryPacket += String(s);
  
  telemetryPacket += "] OBC Temperature: " + String(boardTemp, 2) + " °C";

  runTelemetryTestbench(telemetryPacket, h, m, s, boardTemp);
  delay(1000);
}

// ====================================================================
// 🛑 TESTBENCH SECTION - DO NOT MODIFY THIS CODE 🛑
// This function parses the telemetry string block by block for compliance.
// ====================================================================
void runTelemetryTestbench(String packet, int h, int m, int s, float temp) {
  bool isPassed = true;
  Serial.println("--- Telemetry Packet Verification ---");
  Serial.println("Your Output: " + packet);
  
  packet.trim();

  if (packet == "") {
    Serial.println("❌ ERROR: Telemetry string buffer is empty!");
    isPassed = false;
  } else {
    if (!packet.startsWith("[")) {
      Serial.println("❌ ERROR: Missing starting bracket '['.");
      isPassed = false;
    }
    
    // Check for correct index of closing bracket to ensure [HH:MM:SS] spacing
    int bracketClose = packet.indexOf("]");
    if (bracketClose != 9) {
      Serial.println("❌ ERROR: Timestamp must be exactly 8 characters long [HH:MM:SS]. Check your leading zeros logic!");
      isPassed = false;
    }

    if (packet.indexOf("] OBC Temperature: ") == -1) {
      Serial.println("❌ ERROR: Missing or mistyped string bridge '] OBC Temperature: '. Remember to check your spacing.");
      isPassed = false;
    }

    if (!packet.endsWith("°C") && !packet.endsWith("C")) {
      Serial.println("❌ ERROR: Telemetry data must terminate with the temperature unit '°C'.");
      isPassed = false;
    }
  }

  if (isPassed) {
    Serial.println("✅ [PASS] Telemetry framing and data alignment verified.");
  } else {
    Serial.println("⚠️ [FAIL] Frame compilation errors. Check your padding and bridge syntax.");
  }
  Serial.println("-------------------------------------\n");
}