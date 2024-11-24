#ifndef SENSOR_TO_DIGITAL_HPP
#define SENSOR_TO_DIGITAL_HPP

#include "RadioClass.hpp"

#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP085.h>
#include <Wire.h>


class SensorToDigital : public RadioClass
{
private:
    enum MenuEntries{PREASURE_AT_SEALEVEL, FILTER_RATE, GYRO_RATE, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;

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
    SensorToDigital(TFT_eSPI& newTft, RadioData& newRadioData, Adafruit_MPU6050* newMpu, Adafruit_BMP085* newBmp);
    void doFunction();
    void showValue();

    void showMenu();
    const char * getTitle();
    void up();
    void down();
    bool left();
    bool right();
    void center();
};

SensorToDigital::SensorToDigital(TFT_eSPI& newTft, RadioData& newRadioData, Adafruit_MPU6050* newMpu, Adafruit_BMP085* newBmp):RadioClass(newTft, newRadioData)
{
    mpu = newMpu;
    bmp = newBmp;
}

void SensorToDigital::showMenu()
{
    sprintf(myString,"Pre. Seal. = %4.0fhPa\n", radioData.sensorToDigitalData.seaLevelPressure);
    tft.drawString(myString, posW, posH+incH*0);

    sprintf(myString,"filterRate = %1.2f\n", filterRate);
    tft.drawString(myString, posW, posH+incH*1);

    sprintf(myString,"gyroRate = %1.2f\n", gyroRate);
    tft.drawString(myString, posW, posH+incH*2);

    sprintf(myString,"Pitch = %4.1f\n", radioData.digitalData.pitch);
    tft.drawString(myString, posW, posH+incH*3);

    sprintf(myString,"Roll = %4.1f\n", radioData.digitalData.roll);
    tft.drawString(myString, posW, posH+incH*4);

    sprintf(myString,"Yaw = %3.2f\n", radioData.digitalData.yaw);
    tft.drawString(myString, posW, posH+incH*5);

    sprintf(myString,"Altitude = %4.0fm\n", radioData.digitalData.altitude);
    tft.drawString(myString, posW, posH+incH*6);

    sprintf(myString,"Temperature = %2.1fC\n", radioData.digitalData.temperature);
    tft.drawString(myString, posW, posH+incH*7);

    drawMenuPointer(selectedMenuEntry,NUMBER_OF_MENUENTRIES);
}
const char * SensorToDigital::getTitle()
{
    return "S-To-D";
}
void SensorToDigital::up()
{
    if(selectedMenuEntry > 0) selectedMenuEntry=(MenuEntries)(selectedMenuEntry-1);
    else selectedMenuEntry = NUMBER_OF_MENUENTRIES;
}
void SensorToDigital::down()
{
    if(selectedMenuEntry < NUMBER_OF_MENUENTRIES) selectedMenuEntry=(MenuEntries)(selectedMenuEntry+1);
    else selectedMenuEntry = (MenuEntries)0;
}
bool SensorToDigital::left()
{
    switch (selectedMenuEntry)
    {
    case PREASURE_AT_SEALEVEL:
            if(radioData.sensorToDigitalData.seaLevelPressure > 10) radioData.sensorToDigitalData.seaLevelPressure -= 10;
        break;
    case FILTER_RATE:
            if(filterRate > 0.02) filterRate -= 0.02;
        break;
    case GYRO_RATE:
            if(gyroRate > 0.01) gyroRate -= 0.01;
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}
bool SensorToDigital::right()
{
    switch (selectedMenuEntry)
    {
    case PREASURE_AT_SEALEVEL:
        if(radioData.sensorToDigitalData.seaLevelPressure < 2000) radioData.sensorToDigitalData.seaLevelPressure += 10;
        break;
    case FILTER_RATE:
            if(filterRate < 1) filterRate += 0.02;
        break;
    case GYRO_RATE:
            if(gyroRate < 1) gyroRate += 0.01;
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}
void SensorToDigital::center()
{

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

void SensorToDigital::showValue()
{
    
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