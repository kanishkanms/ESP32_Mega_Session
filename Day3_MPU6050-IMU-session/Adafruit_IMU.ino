#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu; 

void setup() {
  Serial.begin(115200);
  
  if(!mpu.begin(0x69))
  {
    Serial.println("Sensor error! Try again");
    while(1)
    {};
  }
  Serial.println("IMU initiated");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); 
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() 
{
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  Serial.print("Accl X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Accl. Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Accl. Z: ");
  Serial.println(a.acceleration.z);

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");
  delay(500);
}
