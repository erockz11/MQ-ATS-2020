/*
 * 
 * This code is basic usage MPU-6050 Accelerometer and Gyroscope
 * 
 * This code displays data:
 * -Accel X, Accel Y, Accel Z
 * -Accel Angle X, Accel Angle Y,Accel Angle Z,
 * 
 * Library and code have been taken from:
 * https://github.com/tockn/MPU6050_tockn
 * 
 * Updated by Ahmad Shamshiri on July 03, 2018 in Ajax, Ontario, Canada
 * for Robojax.com
 * Get this code from Robojax.com
 * Watch video instruction for this code at:https://youtu.be/uhh7ik02aDc
 * 
 */

void setupAccelerometer(){
  Wire.begin();
  mpu6050.begin();
  Serial.println("Accelerometer ready");
}

void accelerometerLoop() {
  mpu6050.update();
  if(millis() - timer > 100){
    
    Serial.println("=======================================================");
    Serial.print("temp : ");Serial.println(mpu6050.getTemp());
    Serial.print("accX : ");Serial.print(mpu6050.getAccX());
    Serial.print("  accY : ");Serial.print(mpu6050.getAccY());
    Serial.print("  accZ : ");Serial.println(mpu6050.getAccZ());
  
    Serial.print("accAngleX : ");Serial.print(mpu6050.getAccAngleX());
    Serial.print("accAngleY : ");Serial.println(mpu6050.getAccAngleY());
    Serial.println("=======================================================");
    timer = millis();
    
  }

} 
