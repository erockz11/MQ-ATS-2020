enum directionalStates {EAST, WEST};
uint8_t state = EAST;
enum directionalTransitions {NONE, TRACKEND, BUTTON};
uint8_t transition = NONE;

void east();
void west();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  input();
  directionalStateMachine(transition);
  delay(500);
}

void directionalStateMachine(uint8_t i) {
  switch(state){
    case EAST:
      east();
      if(i != NONE){
        transition = NONE;
        state = WEST;
      }
    break;
    case WEST:
      west();
      if(i != NONE){
        transition = NONE;
        state = EAST;
      }
    break;
  }
}

void east(){
  Serial.println("Travelling East");
}

void west(){
  Serial.println("Travelling West");
}

uint8_t input(){
  if (Serial.available() > 0){
    String s = Serial.readStringUntil('\n');
    if(s == "trackend"){
      transition = TRACKEND;
      Serial.println("End reached, changing direction...");
    }
    else if(s == "button"){
      transition = BUTTON;
      Serial.println("Direction change requested, changing direction...");
    }
    else{
      transition = NONE;
    }
  }
}