void setupEncoder(){
  Serial.println("Encoder ready");
}

void encoderLoop(){
  long newPos = enc1.read();
  getRotation(newPos);
  if (newPos != pos) {
    pos = newPos;
    Serial.println((String)"Postion = " + pos);
  }
  else {
    Serial.println((String)"Postion = " + pos);
  }
  delay(1); // delay for stability
}

void getRotation(long newPos){
  if (newPos > pos) {
    Serial.println("Rotating east");
  }
  if (newPos < pos) {
    Serial.println("Rotating west");
  }
  if (newPos == pos) {
    Serial.println("Constant rotation");
  }
}

void getSpeed(){
  
}
