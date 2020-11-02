void setupMotor() {
  Motor.attach(Motor_pin, 1000, 1800);
  Serial.println("Motor ready");
}

void spin(int speedValue) {
  Motor.writeMicroseconds(speedValue);
}
