// NBSPACE Labs: FlatSat Learning Set
// Lab 5.1: Data Acquisition (GPS & RTC)
// Student Code

HardwareSerial gps_uart(???,???);  // Fill pin here
void setup(){
    gps_uart.begin(9600);
    Serial.setTx(???);  // Fill pin here
    Serial.setRx(???);  // Fill pin here
    Serial.begin(115200);
    Serial.println("Hello from OBC!");
}

void loop(){
    while (gps_uart.available()){
        char c = gps_uart.read();
        Serial.write(c);
    }
}