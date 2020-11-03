void setupColour(){
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  digitalWrite(S0,HIGH);  // set frequency-scaling to 20%
  digitalWrite(S1,LOW);
  Serial.println("Colour sensor ready");
}

void senseRed(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  Red = pulseIn(colourOut, LOW);
  Red = map(Red, 25, 72, 255, 0);  // 72 maps to 0, 25 maps to 255 
  Serial.print("R= ");
  Serial.print(Red);
  Serial.print("  ");
}

void senseGreen(){
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  Green = pulseIn(colourOut, LOW);
  Green = map(Green, 30, 90, 255, 0);  // 90 maps to 0, 30 maps to 255 
  Serial.print("G= ");
  Serial.print(Green);
  Serial.print("  ");
}

// Read blue filtered photodiodes
void senseBlue(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  Blue = pulseIn(colourOut, LOW);
  Blue = map(Blue, 25, 70, 255, 0);  // 70 maps to 0, 25 maps to 255 
  Serial.print("B= ");
  Serial.print(Blue);
  Serial.println("  ");
}

void identifyColour(){
  if(Red > Blue && Red > Green && Red > Blue) {
    colourState = RED;    
    Serial.println("Red");   
  }
  if(Green > Blue && Green > Red && Green > -100 && Red < 0) {
    colourState = GREEN;
    Serial.println("Green");   
  }
  if(Blue > Red && Blue > Green && Blue > -100) {
    colourState = BLUE;   
    Serial.println("Blue");   
  }
  if(Red > Blue && Green > Blue && Green > Red & Red > 0 & Green > 0) {
    colourState = YELLOW; 
    Serial.println("Yellow");      
  }
  if(Red < -300 && Green < -200 && Blue < -100) {
    colourState = BLACK;
    Serial.println("Black");   
  }
}

void senseColour(){
  senseRed();
  senseGreen();
  senseBlue();
  identifyColour();
}
