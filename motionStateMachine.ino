enum motionStates {STATIONARY, ACCELERATING, CONSTANT, DECELERATING};
uint8_t state = STATIONARY;
enum motionTransitions {NONE, ESTOP, STOP, START, RED, GREEN, MAXSPEED};
uint8_t transition = NONE;
enum colours {R, G};
uint8_t lastColour = R;

void stationary();
void accelerating();
void constant();
void decelerating();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  input();
  motionStateMachine(transition);
  delay(500);
}

void motionStateMachine(uint8_t i) {
  switch(state){
    case STATIONARY:
      stationary();
      if(i == ESTOP){
        transition = NONE;
        state = STATIONARY;
      }
      if(i == START || i == GREEN){
        transition = NONE;
        state = ACCELERATING;
      }
    break;
    
    case ACCELERATING:
      accelerating();
      if(i == ESTOP){
        transition = NONE;
        state = STATIONARY;
      }
      else if(i == MAXSPEED){
        transition = NONE;
        state = CONSTANT;
      }
      else if(i == STOP || i == GREEN){
        transition = NONE;
        state = DECELERATING;
      }
    break;
    
    case CONSTANT:
      constant();
      if(i == ESTOP){
        transition = NONE;
        state = STATIONARY;
      }
      if(i == STOP || (i == GREEN && lastColour == G)){
        transition = NONE;
        state = DECELERATING;
      }
    break;
    
    case DECELERATING:
      decelerating();
      if(i == ESTOP || i == RED){
        transition = NONE;
        state = STATIONARY;
      }
    break;
  }
}

void stationary(){
  Serial.println("Stopped.");
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

uint8_t input(){
  if (Serial.available() > 0){
    String s = Serial.readStringUntil('\n');
    if(s == "estop"){
      transition = ESTOP;
      Serial.println("Emergency stop.");
    }
    else if(s == "green" && lastColour == R){
      transition = GREEN;
      lastColour = G;
      Serial.println("First green sensed, accelerating...");
    }
    else if(s == "green" && lastColour == G){
      transition = GREEN;
      lastColour = G;
      Serial.println("Second green sensed, decelerating...");
    }
    else if(s == "red"){
      transition = RED;
      lastColour = R;
      Serial.println("Red sensed, stopping...");
    }
    else if(s == "maxspeed"){
      transition = MAXSPEED;
      Serial.println("Maximum speed reached, entering constant acceleration...");
    }
    else if(s == "start"){
      transition = START;
      lastColour = G;
      Serial.println("Start button pressed, accelerating...");
    }
    else if(s == "stop"){
      transition = STOP;
      Serial.println("Stop button pressed, decelerating...");
    }
    else{
      transition = NONE;
    }
  }
}