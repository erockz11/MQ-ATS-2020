//  Code for Train 2 by Team C2
//
//  MASTER Module (Control Box)
//
//  Arduino Uno Pins to HM-10 Bluetooth Pins
//  5V out to BT VCC
//  GND to BT GND
//  D8 (SS RX) to BT TX
//  D9 (SS TX) to BT RX, optimally through a voltage divider (5v to 3.3v)

#include <AltSoftSerial.h>
AltSoftSerial BTserial;

char c = ' ';

void setup(){
  setupBT();
}

void loop(){
  readBT();
  readSerial();
  char commands[] = {'P', 'R', 'T', 'V', '\\', 'X', 'Z'}; // sample command characters
  sendBT(commands[4]);
  delay(100);
}
