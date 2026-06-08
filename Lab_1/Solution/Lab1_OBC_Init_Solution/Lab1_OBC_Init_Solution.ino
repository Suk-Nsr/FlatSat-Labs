
// NBSPACE Labs: FlatSat Learning Set
// Lab 1.1: OBC Initialization
// Solution Code

const int OBC_RX_PIN = PD9; 
const int OBC_TX_PIN = PD8;

String bootMessage = ""; 

void setup() {
  Serial.setRx(OBC_RX_PIN);
  Serial.setTx(OBC_TX_PIN);

  // TODO 1 (FILLED)
  Serial.begin(115200);
  
  while (!Serial){;}

  // TODO 2 (FILLED)
  bootMessage = "OBC Initialized. System Booting...";

  // TODO 3 (FILLED)
  Serial.println(bootMessage);
} 