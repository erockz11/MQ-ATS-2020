/* STATE MACHINE - TRAIN 2
 * Code by T2_C2
 * ENGG3000/2000
 * Macquarie University S2, 2020
 * Joseph
 * Bharosha
 * Jack
 * Maisha
 * Abhinav
 * Francis
 * Scott
 */

/* Variables: Accelerometer */


/* Variables: Bluetooth */

char c = ' ';

/* Variables: Colour Sensor */

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define colourOut 8

int Red = 0;
int Green = 0;
int Blue = 0;

char colours[] = {'R', 'G', 'B', 'Y', 'K'}; 

char colour = ' ';

/* Variables: Motor Encoder */

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

Encoder enc1(31, 33);
long pos = -999;
long previousPos = 0;

/* Variables: Motion State Machine */

/* Variables: Direction State Machine */

void setup() {
  Serial.begin(9600);
  setupAccelerometer();
  setupBluetooth();
  setupColour();
  setupMotorEncoder();
  delay(1000);
}

void loop() {
  senseColour();
  identifyColour();
  sendBT(colour);
  readBT();
  encoderLoop();
  delay(100);
}
