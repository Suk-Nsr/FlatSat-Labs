
// NBSPACE Labs: FlatSat Learning Set
// Lab 3.1: OBC Read EPS Telemetry (Solution)
// Solution Code

#include <Arduino.h>
#include <Wire.h>
#include <PCF85063TP.h>

TwoWire I2C_EPS(PF0, PF1);
PCD85063TP rtc;

#define TMP102_ADDRESS 0x4A

// TODO 1 (FILLED): INA219 Address is 0x40
#define INA219_ADDRESS 0x40

#define INA219_REG_BUS_VOLTAGE 0x02

void setup() {
  Serial.setRx(PD9);
  Serial.setTx(PD8);
  Serial.begin(115200);

  while (!Serial) {;}

  Serial.println("\n=== FlatSat EPS Telemetry Reader Booting ===");

  I2C_EPS.begin();
  Wire.setSDA(PB9);
  Wire.setSCL(PB8);
  Wire.begin();
  rtc.begin();

  Serial.println("EPS telemetry link established.");
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

  // TODO 2 (FILLED): Read Bus Voltage
  float busVoltage = 0.0;
  I2C_EPS.beginTransmission(INA219_ADDRESS);
  I2C_EPS.write(INA219_REG_BUS_VOLTAGE);
  I2C_EPS.endTransmission();
  I2C_EPS.requestFrom(INA219_ADDRESS, 2);
  if (I2C_EPS.available() == 2) {
    byte msb = I2C_EPS.read();
    byte lsb = I2C_EPS.read();
    int raw = (msb << 8) | lsb;
    busVoltage = (raw >> 3) * 0.004;
  }

  String hh = (h < 10) ? "0" + String(h) : String(h);
  String mm = (m < 10) ? "0" + String(m) : String(m);
  String ss = (s < 10) ? "0" + String(s) : String(s);

  String telemetryPacket = "[" + hh + ":" + mm + ":" + ss + "] ";
  telemetryPacket += "TEMP:" + String(boardTemp, 2) + "C | ";
  telemetryPacket += "VBUS:" + String(busVoltage, 2) + "V";

  Serial.println(telemetryPacket);

  delay(2000);
}
