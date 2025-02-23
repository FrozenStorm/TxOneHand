#ifndef SENSOR_TO_DIGITAL_HPP
#define SENSOR_TO_DIGITAL_HPP

#include "RadioClass.hpp"

#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>


class SensorToDigital : public RadioClass
{
private:
    Adafruit_MPU6050*   mpu;
    Adafruit_BMP085*    bmp;
    float               filterRate = 0.8;                
    float               gyroRate = 0.02;
    struct AngleLimit{
        float delta;
        float center;
    };
    AngleLimit angleLimitPitch = {.delta = 45, .center = 0};
    AngleLimit angleLimitRoll = {.delta = 45, .center = 0};
    // TODO  AngleLimit angleLimitYaw = {.delta = 45, .center = 0};

    float analogToDigital(float value, const AngleLimit& limit);
    
public:
    SensorToDigital(RadioData& newRadioData, Adafruit_MPU6050* newMpu, Adafruit_BMP085* newBmp);
    void doFunction();
};

SensorToDigital::SensorToDigital(RadioData& newRadioData, Adafruit_MPU6050* newMpu, Adafruit_BMP085* newBmp):RadioClass(newRadioData)
{
    mpu = newMpu;
    bmp = newBmp;
}

void SensorToDigital::doFunction()
{
    sensors_event_t accel, gyro, temp;
    mpu->getEvent(&accel, &gyro, &temp); // 3ms

    radioData.rawData.gyroPitch = gyro.gyro.z;
    radioData.rawData.gyroRoll = gyro.gyro.y;
    radioData.rawData.gyroYaw = gyro.gyro.x;
    radioData.rawData.accelPitch = -accel.acceleration.z;
    radioData.rawData.accelRoll = -accel.acceleration.y;
    radioData.rawData.accelYaw = -accel.acceleration.x;

    radioData.analogData.gyroPitch = -radioData.rawData.gyroPitch * 180 / PI; // Umrechnen in Grad/s
    radioData.analogData.gyroRoll = radioData.rawData.gyroRoll * 180 / PI;
    radioData.analogData.accelPitch = atan2(radioData.rawData.accelRoll, radioData.rawData.accelYaw) * 180 / PI;
    radioData.analogData.accelRoll = atan2(radioData.rawData.accelPitch, radioData.rawData.accelYaw) * 180 / PI;
    // TODO radioData.analogData.accelYaw = .........

    radioData.analogData.pitch = filterRate * (radioData.analogData.pitch + radioData.analogData.gyroPitch * gyroRate) + (1 - filterRate) * radioData.analogData.accelPitch;
    radioData.analogData.roll = filterRate * (radioData.analogData.roll + radioData.analogData.gyroRoll * gyroRate) + (1 - filterRate) * radioData.analogData.accelRoll;
    // Neues Zentrum setzen wenn der Schalter umgelegt wird.
    if(radioData.digitalData.sideSwitch == 1 && radioData.digitalData.sideSwitchEvent == true)
    {
        angleLimitPitch.center = radioData.analogData.pitch;
        angleLimitRoll.center = radioData.analogData.roll;
        // TODO angleLimitYaw.center = radioData.analogData.yaw;
    }     
    radioData.digitalData.pitch = analogToDigital(radioData.analogData.pitch, angleLimitPitch);
    radioData.digitalData.roll = analogToDigital(radioData.analogData.roll, angleLimitRoll);
    // TODO radioData.digitalData.yaw = .............

    // TODO 90 degree flip fix

    // Removed because very long execution time
    //radioData.digitalData.altitude = filterRate * bmp->readAltitude(radioData.sensorToDigitalData.seaLevelPressure * 100) + (1 - filterRate) * radioData.digitalData.altitude; // 37 ms
    //radioData.digitalData.temperature = bmp->readTemperature(); // 7ms
}

float SensorToDigital::analogToDigital(float value, const AngleLimit& limit)
{
    // Offset wegrechnen
    value = value - limit.center;
    // Umwandeln von analog Bereich zu +/- 1
    value = value / limit.delta;
    // Limitieren auf digital Bereich
    limitValue(value);
    return value;
}

#endif