void setupBT(){
  // initiate communications to HM-10 master module
  
  BTserial.begin(9600);  //setting pin 9 to high
}
// This method will only work if there is no active connection between the modules.\
void forceConnection() {
  BTserial.print("AT+IMME1"); // set Manual Connection Mode -> ON 
  BTserial.print("AT+ROLE1"); // set role to MASTER
  BTserial.print("AT+RESET" ); // reset module
  BTserial.print("AT+COND43639D8A20A"); // connect to physically-specified HM-10 module
  BTserial.print("AT+IMME0"); // set Auto Connection Mode -> ON
  BTserial.print("AT+RESET"); // reset module
}

// Receive status messages from State Machine over Bluetooth.
char readBT(){
  if (BTserial.available()){
    char c = BTserial.read();
    Serial.println((String)"Received: " + c);
    return c;
  }
}

// Send command characters to State Machine over Bluetooth.
void sendBT(char command){
  BTserial.write(command);
  Serial.println((String)"Sent: " + command);
}}
