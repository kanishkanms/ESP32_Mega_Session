#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#define MPU_ADD 0x69 // address of imu 0x69 for our board
Adafruit_MPU6050 mpu;

void imu_setup()
{
    if (!mpu.begin(MPU_ADD))
    {
        Serial.println("Sensor error! Try again");
        while (1)
        {
        };
    }
    Serial.println("IMU initiated");
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // range of accelerometer between -8g to 8g
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);      // range of gyro between -500 to 500 degrees
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);   // setting the cutoff freq of 21Hz
}

void imu_read(float *ax, float *ay, float *az)
{
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    *ax = a.acceleration.x;
    *ay = a.acceleration.y;
    *az = a.acceleration.z;
}
