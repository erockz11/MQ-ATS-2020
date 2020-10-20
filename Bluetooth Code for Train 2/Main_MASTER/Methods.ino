void setupBT(){
  Serial.begin(9600);
  Serial.print("Sketch:   ");   Serial.println(__FILE__);
  Serial.print("Uploaded: ");   Serial.println(__DATE__);
  Serial.println(" ");
  BTserial.begin(9600);  // initiate communications to HM-10 master module
  Serial.println("BTserial started at 9600");  
//  forceConnection();  // only uncomment if connection cannot be established
  delay(1000); // wait for 1 second to ensure connection has settled
}

// This method will only work if there is no active connection between the modules.
void forceConnection() {
  BTserial.print("AT+IMME1"); // set Manual Connection Mode -> ON 
  BTserial.print("AT+ROLE1"); // set role to MASTER
  BTserial.print("AT+RESET" ); // reset module
  BTserial.print("AT+COND43639D8A20A"); // connect to physically-specified HM-10 module
  BTserial.print("AT+IMME0"); // set Auto Connection Mode -> ON
  BTserial.print("AT+RESET"); // reset module
}

// Receive status messages from State Machine over Bluetooth.
void readBT(){
  if (BTserial.available()){
    c = BTserial.read();
    Serial.println((String)"Received: " + c);
  }
}

// Read from the Serial Monitor and send to the Bluetooth module.
void readSerial(){
  if (Serial.available()) {
    c = Serial.read();
    if (c != 10 & c != 13 ){  // Do not send line end characters to the HM-10
      BTserial.write(c);
    }
    Serial.write(c); // Echo user input onto serial monitor.
  }
}

// Send command characters to State Machine over Bluetooth.
void sendBT(char command){
  BTserial.write(command);
  Serial.println((String)"Sent: " + command);
}
