enum directionalStates {EAST, WEST};
uint8_t directionState = EAST;
enum directionalTransitions {NONE, TRACKEND, BUTTON};
uint8_t directionTransition = NONE;

void east();
void west();

void directionLoop() {
  input();
  directionalStateMachine(directionTransition);
  delay(500);
}

void directionalStateMachine(uint8_t i) {
  switch(directionState){
    case EAST:
      east();
      if(i != NONE){
        directionTransition = NONE;
        directionState = WEST;
      }
    break;
    case WEST:
      west();
      if(i != NONE){
        directionTransition = NONE;
        directionState = EAST;
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
    if(colour == 'Y'){
      directionTransition = TRACKEND;
      Serial.println("End reached, changing direction...");
    }
    else if(s == "button"){
      directionTransition = BUTTON;
      Serial.println("Direction change requested, changing direction...");
    }
    else{
      directionTransition = NONE;
    }
  }
}
