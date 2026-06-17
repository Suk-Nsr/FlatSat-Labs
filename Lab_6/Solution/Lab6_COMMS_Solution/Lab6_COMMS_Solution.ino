/*
 * NBSPACE Labs: FlatSat Learning Set
 * Lab 6: Resilient Downlink (COMMS Relay Module)
 * Architecture: Receives RAW data via UART -> Encodes to KISS -> Transmits via RF -> Waits for RF ACK -> Sends UART ACK
 */

#include <Arduino.h>
#include <SPI.h>
#include <RadioLib.h>

// ====================================================================
// HARDWARE PIN DEFINITIONS
// ====================================================================
// Radio SPI Pins (SPI1)
#define RADIO_SCK PA5
#define RADIO_MISO PA6
#define RADIO_MOSI PA7
#define RADIO_NSS PB6
#define RADIO_DIO0 PA10
#define RADIO_RESET PC7
#define RADIO_DIO1 -1

SX1278 radio = new Module(RADIO_NSS, RADIO_DIO0, RADIO_RESET, RADIO_DIO1, SPI);

// UART TO OBC MODULE (PA12 = RX, PA11 = TX)
HardwareSerial ObcUART(PA12, PA11);

// ====================================================================
// KISS PROTOCOL DEFINITIONS & ENCODER
// ====================================================================
#define FEND 0xC0
#define FESC 0xDB
#define TFEND 0xDC
#define TFESC 0xDD

size_t encodeKISS(const uint8_t *payload, size_t payloadSize, uint8_t *outBuffer)
{
  size_t outIndex = 0;
  outBuffer[outIndex++] = FEND;
  for (size_t i = 0; i < payloadSize; i++)
  {
    uint8_t currentByte = payload[i];
    if (currentByte == FEND)
    {
      outBuffer[outIndex++] = FESC;
      outBuffer[outIndex++] = TFEND;
    }
    else if (currentByte == FESC)
    {
      outBuffer[outIndex++] = FESC;
      outBuffer[outIndex++] = TFESC;
    }
    else
    {
      outBuffer[outIndex++] = currentByte;
    }
  }
  outBuffer[outIndex++] = FEND;
  return outIndex;
}

// ====================================================================
// SETUP FUNCTION
// ====================================================================
void setup()
{
  Serial.begin(115200);  // Debug to PC
  ObcUART.begin(115200); // Communication to OBC

  delay(2000);
  Serial.println("\n=== FlatSat COMMS: Radio Relay Active ===");

  SPI.setMISO(RADIO_MISO);
  SPI.setMOSI(RADIO_MOSI);
  SPI.setSCLK(RADIO_SCK);
  SPI.begin();

  Serial.print("[SYSTEM] Initializing RF Module... ");
  if (radio.beginFSK() == RADIOLIB_ERR_NONE)
  {
    radio.setFrequency(433.0);
    radio.setBitRate(9.6);
    radio.setOutputPower(2);
    Serial.println("OK!");
  }
  else
  {
    Serial.println("FAILED!");
    while (true)
      ;
  }

  Serial.println("[SYSTEM] Flushing phantom UART data...");
  delay(500); // รอให้ระบบนิ่งสนิทครึ่งวินาที
  while (ObcUART.available())
  {
    ObcUART.read();
  }
  Serial.println("[SYSTEM] System Ready.");
}

// ====================================================================
// MAIN LOOP FUNCTION
// ====================================================================
void loop()
{
  // 1. รอรับเฉพาะ Header ก่อน (ต้องการแค่ 3 Bytes: ChunkID High, ChunkID Low, Length)
  if (ObcUART.available() >= 3)
  {

    uint8_t header[3];
    ObcUART.readBytes(header, 3);

    uint16_t chunkId = (header[0] << 8) | header[1];
    uint8_t payloadLen = header[2]; // นี่ไง! รู้แล้วว่าก้อนนี้มีรูปกี่ไบต์

    // คำนวณขนาดทั้งหมดที่จะต้องรับเพิ่ม (รูป + Checksum อีก 1)
    size_t expectedRemaining = payloadLen;

    uint8_t rawPayload[100]; // เตรียม Buffer ไว้ใหญ่ๆ เลย

    // เอา Header ยัดกลับเข้าไปใน Buffer คืน
    rawPayload[0] = header[0];
    rawPayload[1] = header[1];
    rawPayload[2] = header[2];

    // ตั้งเวลารอรับส่วนที่เหลือ
    ObcUART.setTimeout(250);
    size_t rxCount = ObcUART.readBytes(&rawPayload[3], expectedRemaining);

    if (rxCount == expectedRemaining)
    {

      size_t totalRawSize = 3 + expectedRemaining;

      // 2. คำนวณ Checksum (XOR) ของข้อมูลทั้งหมด
      uint8_t checksum = 0;
      for (size_t i = 0; i < totalRawSize; i++)
      {
        checksum ^= rawPayload[i];
      }
      rawPayload[totalRawSize] = checksum; // แนบ Checksum ไปที่ตำแหน่งสุดท้าย

      // 3. ห่อหุ้มด้วยโปรโตคอล KISS
      uint8_t txKISSBuffer[150];
      size_t kissPacketSize = encodeKISS(rawPayload, totalRawSize + 1, txKISSBuffer);

      Serial.print("[RELAY] Encoded Chunk #");
      Serial.print(chunkId);
      Serial.print(" (Len: ");
      Serial.print(payloadLen);
      Serial.print(") into KISS (");
      Serial.print(kissPacketSize);
      Serial.print(" bytes). Transmitting... ");

      // 4. ยิงคลื่นวิทยุ
      // (เราเพิ่มการดักจับ Error ตรงนี้ให้ด้วย จะได้รู้ถ้ายิงไม่สำเร็จ)
      int txState = radio.transmit(txKISSBuffer, kissPacketSize);
      if (txState != RADIOLIB_ERR_NONE)
      {
        Serial.print("RF Error: ");
        Serial.println(txState);
      }

      // 5. รอรับสัญญาณตอบกลับ (ACK)
      String rfResponse;
      int rxState = radio.receive(rfResponse, 1000);

      if (rxState == RADIOLIB_ERR_NONE && rfResponse.indexOf("ACK") >= 0)
      {
        Serial.println(" -> Got RF ACK!");
        ObcUART.println("ACK");
      }
      else
      {
        Serial.println(" -> RF Timeout/Drop.");
      }

      // ล้างข้อมูลขยะ (EMI Noise)
      while (ObcUART.available())
      {
        ObcUART.read();
      }
    }
    else
    {
      Serial.println("[ERROR] UART Payload fragmented. Flushing buffer.");
      while (ObcUART.available())
        ObcUART.read();
    }
  }
}