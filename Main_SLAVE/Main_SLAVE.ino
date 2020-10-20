//  Code for Train 2 by Team C2 
//
//  SLAVE Module (State Machine)
//
//  Arduino Mega Pins to HM-10 Bluetooth Pins
//  5V out to BT VCC
//  GND to BT GND
//  Comm 18 (TX1) to BT RX, optimally through a voltage divider (5v to 3.3v)
//  Comm 19 (RX1) to BT TX

char c=' ';
 
void setup(){
  setupBT();
}
 
void loop(){
  readBT();
  readSerial(); 
  char status[] = {'1', '2', '3', '4', '5'}; // sample status codes
  for(int j = 0; j < sizeof(status); j++){
    sendBT(status[j]);
    delay(100);
  }
}
