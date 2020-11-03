/* ---------------------------------*\
|    STATE MACHINE - TRAIN 2         |
|    Code by T2_C2                   |
|    ENGG3000/2000                   |
|    Macquarie University S2, 2020   |
|    > Joseph                        |
|    > Bharosha                      |
|    > Jack                          |
|    > Maisha                        |
|    > Abhinav                       |
|    > Francis                       |
|    > Scott                         |
\*----------------------------------*/

/************ Declarations: Bluetooth *************/

char command = ' ';
char pCommand = ' ';

/********** Declarations: Colour Sensor ***********/

#define S0 4
#define S1 5
#define S2 6 
#define S3 7
#define colourOut 8
int Red = 0;
int Green = 0;
int Blue = 0;
enum colourStates {RED, GREEN, BLUE, YELLOW, BLACK};
uint8_t colourState= RED;
uint8_t pColourState= RED;

/************* Declarations: Motor ***************/

#include <Servo.h> 
int Motor_pin = 9;
Servo Motor;
int maxS = 1000;
int slowS = 1300;
int maxR = 1800;
int slowR = 1600;
int stopV = 1500;

/********* Declarations: State Machine ***********/

enum directionStates {EAST, WEST};
uint8_t directionState = EAST;
boolean emergency = false;
enum operationStates {HALTED, MOVING};
uint8_t operationState= HALTED;
char tStatus = ' ';
int greenCounter = 0;
int baud = 9600;

/**************************************************/


/*-------------------------------*/
/*          CODE PROPER          */
/*-------------------------------*/

void setup() {
  Serial.begin(baud);
  setupBluetooth();
  setupColour();
  setupMotor();
  delay(1000);
}

void loop() {
 // spin(1800);
  if(emergency == false){
    spin(maxS);
    readBT();
    senseColour();
    checkForUpdates();
  }
}
