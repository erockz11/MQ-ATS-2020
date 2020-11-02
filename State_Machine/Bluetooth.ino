void setupBluetooth() {
  Serial1.begin(baud);
  Serial.println("Bluetooth ready");
}

void forceConnection() {
  Serial1.print("AT+IMME1");
  Serial1.print("AT+RESET" );
  delay(5000);
  Serial1.print("AT+IMME0");
  Serial1.print("AT+RESET");
}

void readBT() {
  if (Serial1.available()) {
    command = Serial1.read();
    Serial.println((String)"Received: " + command);
  }
  else {
    forceConnection();
  }
}

void sendBT(char tStatus) {
  Serial1.write(tStatus);
  Serial.println((String)"Sent: " + tStatus);
}
