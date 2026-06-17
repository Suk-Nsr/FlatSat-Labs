import serial
import time
import binascii

# ⚠️ กรุณาเปลี่ยน COM Port ให้ตรงกับบอร์ด Ground Station ของคุณ
COM_PORT = 'COM12' # สำหรับ Windows (ถ้าเป็น Mac/Linux จะเป็นประมาณ '/dev/tty.usbserial...')
BAUD_RATE = 115200

OUTPUT_FILE = "received_photo.jpg"
CHUNK_SIZE = 48

try:
    ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=1)
    print(f"Ground Station Control Terminal Connected on {COM_PORT}")
    print(f"Waiting for Satellite Downlink... Image will be saved to '{OUTPUT_FILE}'\n")
    
    # เปิดไฟล์ด้วยโหมด r+b (เขียนทับและกระโดดไปตามตำแหน่งได้) หรือสร้างใหม่ถ้ายังไม่มี
    try:
        f = open(OUTPUT_FILE, "r+b")
    except FileNotFoundError:
        f = open(OUTPUT_FILE, "wb")
        
    with f as file:
        while True:
            # อ่านข้อความจากสาย USB ที่มาจากบอร์ด GS
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            
            if "CHUNK_DATA:" in line:
                # แยกเอาเฉพาะข้อความ Hex ที่อยู่หลังเครื่องหมาย :
                hex_data = line.split("CHUNK_DATA:")[1]
                
                # ถอด ID 2 ไบต์แรก (4 ตัวอักษร Hex) ออกมา
                chunk_id_hex = hex_data[:4]
                payload_len_hex = hex_data[4:6]
                image_hex = hex_data[6:]
                
                chunk_index = int(chunk_id_hex, 16)
                payload_len = int(payload_len_hex, 16)
                image_bytes = binascii.unhexlify(image_hex)
                
                # กระโดด (Seek) ไปยังตำแหน่งเป๊ะๆ ของภาพก้อนนี้ แล้วเขียนลงไป
                file.seek(chunk_index * CHUNK_SIZE)
                file.write(image_bytes)
                file.flush() # บังคับบันทึกลงฮาร์ดดิสก์ทันที
                
                print(f"[DOWNLOAD] Chunk #{chunk_index:03d} successfully written to disk. ({len(image_bytes)} bytes)")
            
            elif line != "":
                # พิมพ์ Log ทั่วไปที่ Ground Station พ่นออกมา
                print(line)
                
except serial.SerialException as e:
    print(f"Serial Connection Error: Please check your COM Port. Details: {e}")
except KeyboardInterrupt:
    print("\nMission Terminated by Operator.")