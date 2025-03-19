#pragma once

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

    float analogToDigital(float value, const RadioData::SensorToDigitalData::AngleLimit& limit);
    void updateOrientation();
    void selectAxis(float* accelPitch, float* accelRoll, float* accelYaw, float* gyroPitch, float* gyroRoll, float* gyroYaw);
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
    float accelPitch, accelRoll, accelYaw;
    float gyroPitch, gyroRoll, gyroYaw;

    // RAW Einlesen
    radioData.rawData.gyroX = gyro.gyro.x;
    radioData.rawData.gyroY = -gyro.gyro.z;
    radioData.rawData.gyroZ = -gyro.gyro.y;
    radioData.rawData.accelX = -accel.acceleration.x;
    radioData.rawData.accelY = -accel.acceleration.z;
    radioData.rawData.accelZ = accel.acceleration.y;

    updateOrientation();

    selectAxis(&accelPitch, &accelRoll, &accelYaw, &gyroPitch, &gyroRoll, &gyroYaw);

    // Zu analog umrechnen
    radioData.analogData.gyroPitch = gyroPitch * 180 / PI;
    radioData.analogData.gyroRoll = gyroRoll * 180 / PI;
    radioData.analogData.gyroYaw = gyroYaw * 180 / PI;

    if(accelPitch != NAN && accelYaw != NAN)
    {
        radioData.analogData.accelPitch = atan2(accelPitch, accelYaw) * 180 / PI;
        if(radioData.analogData.accelPitch == NAN) radioData.analogData.accelPitch = 0;
    }
    if(accelRoll != NAN && accelYaw != NAN)
    {
        radioData.analogData.accelRoll = atan2(accelRoll, accelYaw) * 180 / PI;
        if(radioData.analogData.accelRoll == NAN) radioData.analogData.accelRoll = 0;
    }
    // TODO radioData.analogData.accelYaw = .........

    radioData.analogData.pitch = filterRate * (radioData.analogData.pitch + radioData.analogData.gyroPitch * gyroRate) + (1 - filterRate) * radioData.analogData.accelPitch;
    radioData.analogData.roll = filterRate * (radioData.analogData.roll + radioData.analogData.gyroRoll * gyroRate) + (1 - filterRate) * radioData.analogData.accelRoll;
    
    radioData.digitalData.pitch = analogToDigital(radioData.analogData.pitch, radioData.sensorToDigitalData.angleLimitPitch);
    radioData.digitalData.roll = analogToDigital(radioData.analogData.roll, radioData.sensorToDigitalData.angleLimitRoll);
    // TODO radioData.digitalData.yaw = .............

    // TODO 90 degree flip fix

    // Removed because very long execution time
    //radioData.digitalData.altitude = filterRate * bmp->readAltitude(radioData.sensorToDigitalData.seaLevelPressure * 100) + (1 - filterRate) * radioData.digitalData.altitude; // 37 ms
    //radioData.digitalData.temperature = bmp->readTemperature(); // 7ms
}

void SensorToDigital::selectAxis(float* accelPitch, float* accelRoll, float* accelYaw, float* gyroPitch, float* gyroRoll, float* gyroYaw)
{
    switch (radioData.digitalData.orientation)
    {
    case RadioData::Orientation::T_UP:
        *accelPitch = radioData.rawData.accelX;
        *accelRoll = radioData.rawData.accelY;
        *accelYaw = radioData.rawData.accelZ;
        *gyroPitch = radioData.rawData.gyroY;
        *gyroRoll = radioData.rawData.gyroX;
        *gyroYaw = radioData.rawData.gyroZ;
        break;
    case RadioData::Orientation::T_LEFT:
        *accelPitch = radioData.rawData.accelX;
        *accelRoll = radioData.rawData.accelZ;
        *accelYaw = -radioData.rawData.accelY;
        *gyroPitch = -radioData.rawData.gyroZ;
        *gyroRoll = radioData.rawData.gyroX;
        *gyroYaw = radioData.rawData.gyroY;
        break;
    case RadioData::Orientation::T_RIGHT:
        *accelPitch = radioData.rawData.accelX;
        *accelRoll = -radioData.rawData.accelZ;
        *accelYaw = radioData.rawData.accelY;
        *gyroPitch = radioData.rawData.gyroZ;
        *gyroRoll = radioData.rawData.gyroX;
        *gyroYaw = -radioData.rawData.gyroY;
        break;
    case RadioData::Orientation::T_LEFT_DOWN:
        *accelPitch = radioData.rawData.accelY;
        *accelRoll = radioData.rawData.accelZ;
        *accelYaw = radioData.rawData.accelX;
        *gyroPitch = -radioData.rawData.gyroZ;
        *gyroRoll = -radioData.rawData.gyroY;
        *gyroYaw = radioData.rawData.gyroX;
        break;
    case RadioData::Orientation::T_LEFT_UP:
        *accelPitch = -radioData.rawData.accelY;
        *accelRoll = radioData.rawData.accelZ;
        *accelYaw = -radioData.rawData.accelX;
        *gyroPitch = -radioData.rawData.gyroZ;
        *gyroRoll = radioData.rawData.gyroY;
        *gyroYaw = -radioData.rawData.gyroX;
        break;
    }
}

void SensorToDigital::updateOrientation()
{
    if(radioData.functionData.armed == false) // TODO wenn im armed mode und orientation wechselt dann steuerung auf altem wert halten damit kein flipping entsteht.
    {
        if(abs(radioData.rawData.accelZ) > abs(radioData.rawData.accelX) && abs(radioData.rawData.accelZ) > abs(radioData.rawData.accelY))
        {
            if(radioData.rawData.accelZ > 0)
            {
                radioData.digitalData.orientation = RadioData::Orientation::T_UP;
            }
            else
            {
                radioData.digitalData.orientation = RadioData::Orientation::T_DOWN;
            }
        }
        else if(abs(radioData.rawData.accelX) > abs(radioData.rawData.accelY))
        {
            if(radioData.rawData.accelX > 0)
            {
                radioData.digitalData.orientation = RadioData::Orientation::T_LEFT_DOWN;
            }
            else
            {
                radioData.digitalData.orientation = RadioData::Orientation::T_LEFT_UP;
            }
        }
        else
        {
            if(radioData.rawData.accelY > 0)
            {
                radioData.digitalData.orientation = RadioData::Orientation::T_RIGHT;
            }
            else
            {
                radioData.digitalData.orientation = RadioData::Orientation::T_LEFT;
            }
        }
    }
}

float SensorToDigital::analogToDigital(float value, const RadioData::SensorToDigitalData::AngleLimit& limit)
{
    // Offset wegrechnen
    value = value - limit.center;
    // Umwandeln von analog Bereich zu +/- 1
    value = value / limit.delta;
    // Limitieren auf digital Bereich
    limitValue(value);
    return value;
}