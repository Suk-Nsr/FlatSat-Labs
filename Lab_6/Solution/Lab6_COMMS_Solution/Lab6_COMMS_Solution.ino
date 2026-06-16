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
    // 1. รอรับข้อมูลดิบจาก OBC (แค่มีเข้ามา 1 ไบต์ก็เริ่มทำงานเลย เพื่อไม่ให้ Buffer ล้น)
    if (ObcUART.available() > 0)
    {

        uint8_t rawPayload[67]; // 66 bytes data + 1 byte Checksum

        // ตั้งเวลารอข้อมูลให้มาครบ 66 ไบต์ ภายใน 250 มิลลิวินาที
        ObcUART.setTimeout(250);

        // ทยอยดูดข้อมูลออกจาก Serial Buffer จนครบ 66 ไบต์
        size_t rxCount = ObcUART.readBytes(rawPayload, 66);

        if (rxCount == 66)
        {
            // 2. คำนวณ Checksum (XOR) ของข้อมูล 66 ไบต์
            uint8_t checksum = 0;
            for (int i = 0; i < 66; i++)
            {
                checksum ^= rawPayload[i];
            }
            rawPayload[66] = checksum; // แนบ Checksum ไปที่ตำแหน่งสุดท้าย

            // 3. ห่อหุ้มด้วยโปรโตคอล KISS
            uint8_t txKISSBuffer[135];
            size_t kissPacketSize = encodeKISS(rawPayload, 67, txKISSBuffer);

            Serial.print("[RELAY] Encoded Chunk #");
            uint16_t chunkId = (rawPayload[0] << 8) | rawPayload[1];
            Serial.print(chunkId);
            Serial.print(" into KISS (");
            Serial.print(kissPacketSize);
            Serial.println(" bytes). Transmitting to Earth...");

            // 4. ยิงคลื่นวิทยุออกสู่สภาวะอวกาศ
            radio.transmit(txKISSBuffer, kissPacketSize);

            // 5. เปิดหูฟัง รอรับสัญญาณตอบกลับ (ACK) จาก Ground Station
            String rfResponse;
            int rxState = radio.receive(rfResponse, 1000); // รอพื้นดินตอบ 1 วินาที

            if (rxState == RADIOLIB_ERR_NONE && rfResponse.indexOf("ACK") >= 0)
            {
                Serial.println("        -> 🟢 Got RF ACK from Ground Station!");
                // 6. เมื่อยืนยันว่าถึงโลกอย่างปลอดภัยแล้ว ให้บอก OBC ว่า "ACK"
                ObcUART.println("ACK");
            }
            else
            {
                Serial.println("        -> 🔴 RF Timeout / Drop. (Ignoring OBC, forcing retransmit)");
                // ไม่ส่งอะไรกลับไปให้ OBC เพื่อให้ OBC เกิด Timeout และบังคับส่งซ้ำ
            }
            while (ObcUART.available())
            {
                ObcUART.read();
            }
        }
        else
        {
            // กรณีมาไม่ครบ 66 ไบต์ (เช่นมีสัญญาณกวนในสาย) ให้ล้างข้อมูลขยะทิ้งไป
            Serial.println("[ERROR] UART Payload fragmented. Flushing buffer.");
            while (ObcUART.available())
                ObcUART.read();
        }
    }
}