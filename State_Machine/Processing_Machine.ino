/* The 'Servo.h' library allows you to control motors and servos by passing any value between
0 and 180 into the "write" method (Motor.write() in this program).
For a motor:   0 is full power in one direction,
             180 is full direction in the opposite direction,
              90 is stop.
For a servo:   0 is all the way one direction,
             180 is all the way in the opposite direction,
              90 is the middle.
*/

int Motor_max_forward = 1000;
int Motor_max_reverse = 2000;
int Motor_stop = 1500; 
unsigned long next_step;
unsigned long last_step;
int state = 0;

// Motion State Machine Declarations
enum motionStates {STATIONARY, ACCELERATING, CONSTANT, DECELERATING};
uint8_t motionState = STATIONARY;
enum motionTransitions {MNONE, ESTOP, STOP, START, RED, GREEN, MAXSPEED};
uint8_t motionTransition = MNONE;
enum colours {R, G, Y};
uint8_t lastColour = R;

void stationary();
void accelerating();
void constant();
void decelerating();

// Directional State Machine Declarations
enum directionalStates {EAST, WEST};
uint8_t directionalState = EAST;
enum directionalTransitions {DNONE, TRACKEND, BUTTON};
uint8_t directionalTransition = DNONE;

void east();
void west();

void setupMotor() {
  // Initialize Serial
  pinMode(led, OUTPUT);     // setup the LED to blink as we run the loop
  pinMode(Motor0_On_Pin, OUTPUT);

  Motor0.attach(Motor_pin0, 1000, 2000);  // This pin is the control signal for the Motor
  Motor0.writeMicroseconds(Motor_stop);
  Serial.println("Motor ready");
}

void motionLoop() {
  // Inputs
  input();
  // Motion State Machine
  motionStateMachine(motionTransition);
  // Directional State Machine
  directionalStateMachine(directionalTransition);
  // Arbitrary delay to make serial monitor readable
  delay(250);
}

// Motion State Machine
void motionStateMachine(uint8_t i) {
  switch(motionState){
    case STATIONARY:
      stationary();
      if(i == ESTOP){
        motionTransition = MNONE;
        motionState = STATIONARY;
      }
      if(i == START || i == GREEN){
        motionTransition = MNONE;
        motionState = ACCELERATING;
      }
    break;
    
    case ACCELERATING:
      accelerating();
      if(i == ESTOP){
        motionTransition = MNONE;
        motionState = STATIONARY;
      }
      else if(i == MAXSPEED){
        motionTransition = MNONE;
        motionState = CONSTANT;
      }
      else if(i == STOP || i == GREEN){
        motionTransition = MNONE;
        motionState = DECELERATING;
      }
    break;
    
    case CONSTANT:
      constant();
      if(i == ESTOP){
        motionTransition = MNONE;
        motionState = STATIONARY;
      }
      if(i == STOP || (i == GREEN && lastColour == G)){
        motionTransition = MNONE;
        motionState = DECELERATING;
      }
    break;
    
    case DECELERATING:
      decelerating();
      if(i == ESTOP || i == RED){
        motionTransition = MNONE;
        motionState = STATIONARY;
      }
    break;
  }
}

// Motion Methods
void stationary(){
  Serial.println("Stopped.");
  digitalWrite(Motor0_On_Pin, HIGH);
  Motor0.writeMicroseconds(Motor_stop);
}

void accelerating(){
  Serial.println("Accelerating.");
}

void constant(){
  Serial.println("Max speed.");
}

void decelerating(){
  Serial.println("Decelerating.");
}

// Directional State Machine
void directionalStateMachine(uint8_t i) {
  switch(directionalState){
    case EAST:
      east();
      if(i != DNONE){
        directionalTransition = DNONE;
        directionalState = WEST;
      }
    break;
    case WEST:
      west();
      if(i != DNONE){
        directionalTransition = DNONE;
        directionalState = EAST;
      }
    break;
  }
}

// Directional Methods
void east(){
  Serial.println("Travelling East");
  digitalWrite(Motor0_On_Pin, HIGH);
  Motor0.writeMicroseconds(Motor_max_forward);
}

void west(){  
  Serial.println("Travelling West");
  digitalWrite(Motor0_On_Pin, HIGH);
  Motor0.writeMicroseconds(Motor_max_reverse);
}

// Inputs: Type these strings into the command line to change between states
uint8_t input(){
  if (Serial.available() > 0){
    String s = Serial.readStringUntil('\n');
    if(s == "trackend"){
      directionalTransition = TRACKEND;
      Serial.println("End reached, changing direction...");
    }
    else if(s == "button"){
      directionalTransition = BUTTON;
      Serial.println("Direction change requested, changing direction...");
    }
    else{
      directionalTransition = DNONE;
    }
    if(s == "estop"){
      motionTransition = ESTOP;
      Serial.println("Emergency stop.");
    }
    else if(s == "green" && lastColour == R){
      motionTransition = GREEN;
      lastColour = G;
      Serial.println("First green sensed, accelerating...");
    }
    else if(s == "green" && lastColour == G){
      motionTransition = GREEN;
      lastColour = G;
      Serial.println("Second green sensed, decelerating...");
    }
    else if(s == "red"){
      motionTransition = RED;
      lastColour = R;
      Serial.println("Red sensed, stopping...");
    }
    else if(s == "maxspeed"){
      motionTransition = MAXSPEED;
      Serial.println("Maximum speed reached, entering constant acceleration...");
    }
    else if(s == "start"){
      motionTransition = START;
      lastColour = G;
      Serial.println("Start button pressed, accelerating...");
    }
    else if(s == "stop"){
      motionTransition = STOP;
      Serial.println("Stop button pressed, decelerating...");
    }
    else{
      motionTransition = MNONE;
      directionalTransition = DNONE;
    }
  }
}
