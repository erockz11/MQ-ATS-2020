void setupColour(){
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(BlackLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(BlueLED, OUTPUT);
  pinMode(YellowLED, OUTPUT);
  pinMode(RedLED, OUTPUT);
  digitalWrite(S0,HIGH);  // set frequency-scaling to 20%
  digitalWrite(S1,LOW);
  Serial.println("Colour sensor ready");
}

// Read red filtered photodiodes
void senseRed(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  Red = pulseIn(colourOut, LOW);
  Red = map(Red, 25, 72, 255, 0);  // 72 maps to 0, 25 maps to 255 
  Serial.print("R= ");
  Serial.print(Red);  // print RED color frequency
  Serial.print("  ");
}

// Read green filtered photodiodes
void senseGreen(){
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  Green = pulseIn(colourOut, LOW);
  Green = map(Green, 30, 90, 255, 0);  // 90 maps to 0, 30 maps to 255 
  Serial.print("G= ");
  Serial.print(Green);  //print GREEN color frequency
  Serial.print("  ");
}

// Read blue filtered photodiodes
void senseBlue(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  Blue = pulseIn(colourOut, LOW);
  Blue = map(Blue, 25, 70, 255, 0);  // 70 maps to 0, 25 maps to 255 
  Serial.print("B= ");
  Serial.print(Blue);  // print BLUE color frequency
  Serial.println("  ");
}

void senseColour(){
  senseRed();
  senseGreen();
  senseBlue();
  identifyColour();
}

void identifyColour(){
  if(Red > Blue && Red > Green && (Red-Green) > 110 && Blue < 0 && (Red-Blue) > 170 && Green < 100) {
    colour = colours[0];
    digitalWrite(RedLED,HIGH);
    digitalWrite(GreenLED,LOW);
    digitalWrite(BlueLED,LOW);
    digitalWrite(YellowLED,LOW);
    digitalWrite(BlackLED,LOW);      
  }
  if(Green > Blue && Green > Red && Blue < 100 && Red < 300) {
    colour = colours[1];
    digitalWrite(GreenLED,HIGH);
    digitalWrite(RedLED,LOW);
    digitalWrite(BlueLED,LOW);
    digitalWrite(YellowLED,LOW);
    digitalWrite(BlackLED,LOW);     
  }
  if(Blue > Red && Blue > Green) {
    colour = colours[2];
    digitalWrite(BlueLED,HIGH);
    digitalWrite(RedLED,LOW);
    digitalWrite(GreenLED,LOW);
    digitalWrite(YellowLED,LOW);
    digitalWrite(BlackLED,LOW);    
  }
  if(Red > Blue && Green > Blue && Green > 0 && (Red-Green) < 90 && (Red-Blue) > 150 || Red > 300) {
    colour = colours[3];  
    digitalWrite(YellowLED,HIGH);
    digitalWrite(RedLED,LOW);
    digitalWrite(GreenLED,LOW);
    digitalWrite(BlueLED,LOW);
    digitalWrite(BlackLED,LOW);      
  }
  if(Red < 0 && Green < 0 && Blue < 0) {
    colour = colours[4];
    digitalWrite(BlackLED,HIGH);
    digitalWrite(RedLED,LOW);
    digitalWrite(GreenLED,LOW);
    digitalWrite(BlueLED,LOW);
    digitalWrite(YellowLED,LOW);  
  }
}
