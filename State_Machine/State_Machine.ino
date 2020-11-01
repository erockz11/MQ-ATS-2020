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

/* Declarations: Accelerometer */

#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);
long timer = 0;

/* Declarations: Bluetooth */

char c = ' ';

/* Declarations: Colour Sensor */

#define S0 4
#define S1 5
#define S2 6 
#define S3 7
#define colourOut 8
#define BlackLED 22
#define GreenLED 23
#define BlueLED 24
#define YellowLED 25
#define RedLED 26

int Red = 0;
int Green = 0;
int Blue = 0;

char colours[] = {'R', 'G', 'B', 'Y', 'K'}; 

char colour = ' ';

/* Declarations: Motor Encoder */

#define ENCODER_DO_NOT_USE_INTERRUPTS
#include <Encoder.h>

Encoder enc1(31, 33);
long pos = -999;
long previousPos = 0;

/* Declarations: Motion State Machine */

/* Declarations: Direction State Machine */

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
  readBT();
  sendBT(colour);
  encoderLoop();
  //accelerometerLoop();
}
