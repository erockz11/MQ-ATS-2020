void setupColour(){
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(colourOut, INPUT);
  digitalWrite(S0,HIGH);  // set frequency-scaling to 20%
  digitalWrite(S1,LOW);
  Serial.println("Colour sensor ready");
}

// Read red filtered photodiodes
void senseRed(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  R = pulseIn(colourOut, LOW);
  R = map(R, 25, 72, 255, 0);  // 72 maps to 0, 25 maps to 255 
  Serial.print("R= ");
  Serial.print(R);  // print RED color frequency
  Serial.print("  ");
}

// Read green filtered photodiodes
void senseGreen(){
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  G = pulseIn(colourOut, LOW);
  G = map(G, 30, 90, 255, 0);  // 90 maps to 0, 30 maps to 255 
  Serial.print("G= ");
  Serial.print(G);  //print GREEN color frequency
  Serial.print("  ");
}

// Read blue filtered photodiodes
void senseBlue(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  B = pulseIn(colourOut, LOW);
  B = map(B, 25, 70, 255, 0);  // 70 maps to 0, 25 maps to 255 
  Serial.print("B= ");
  Serial.print(B);  // print BLUE color frequency
  Serial.println("  ");
}

void senseColour(){
  senseRed();
  senseGreen();
  senseBlue();
  identifyColour();
}

void identifyColour(){
  if(R > B && R > G) {
    colour = colours[0];  
  }
  if(G > B && G > R) {
    colour = colours[1];  
  }
  if(B > R && B > G) {
    colour = colours[2];
  }
  if(R > B && G > B) {
    colour = colours[3];    
  }
  if(R < 0 && G < 0 && B < 0) {
    colour = colours[4];
  }
}
