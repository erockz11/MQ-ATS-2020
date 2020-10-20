#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

//there are 24 ticks in one revolution

// Beware of Serial.print() speed.  Without interrupts, if you
// transmit too much data with Serial.print() it can slow your
// reading from Encoder.  Arduino 1.0 has improved transmit code.
// Using the fastest baud rate also helps.  Teensy has USB packet
// buffering.  But all boards can experience problems if you print
// too much and fill up buffers.

// Change these two numbers to the pins connected to your encoder.
//   With ENCODER_DO_NOT_USE_INTERRUPTS, no interrupts are ever
//   used, even if the pin has interrupt capability
Encoder myEnc(5, 6);
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(9600);
  Serial.println("Basic NoInterrupts Test:");
}

long position  = -999;

void loop() {
  long newPos = myEnc.read();
  if (newPos != position) {
    position = newPos;
    Serial.println(position);
  }
  // With any substantial delay added, Encoder can only track
  // very slow motion.  You may uncomment this line to see
  // how badly a delay affects your encoder.
  //delay(50);
}