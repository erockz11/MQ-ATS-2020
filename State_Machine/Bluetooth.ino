void setupBluetooth(){
  Serial1.begin(9600); //Initiate communications to HM-10 slave module  
  Serial.println("Bluetooth ready");
}

// This method will only work if there is no active connection between the modules.
void forceConnection() {
  Serial1.print("AT+IMME1"); // set Manual Connection Mode -> ON 
  Serial1.print("AT+RESET" ); // reset module
  delay(5000);  // wait for 5 seconds, ensure this method is also called on other module
  Serial1.print("AT+IMME0"); // set Auto Connection Mode -> ON
  Serial1.print("AT+RESET"); // reset module
}

// Receive command messages from Control Box over Bluetooth.
void readBT(){
  if (Serial1.available()){
    c = Serial1.read();
    Serial.println((String)"Received: " + c);
  }
}

// Read from the Serial Monitor and send to the Bluetooth module.
void readSerial(){
  if (Serial.available()) {
    c = Serial.read();
    if (c != 10 & c != 13 ){  // Do not send line end characters to the HM-10
      Serial1.write(c);
    }
    Serial.println((String)"Typed: " + c); // Echo user input onto serial monitor.
  }
}

// Send status characters to Control Box over Bluetooth.
void sendBT(char state){
  Serial1.write(state);
  Serial.println((String)"Sent: " + state);
}
