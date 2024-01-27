#ifndef SENSOR_TO_DIGITAL_HPP
#define SENSOR_TO_DIGITAL_HPP

#include "RadioClass.hpp"

//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_LSM303_U.h>
//#include <Adafruit_BMP085_U.h>
//#include <Adafruit_L3GD20_U.h>
//#include <Adafruit_10DOF.h>

// https://github.com/kriswiner/MPU6050HMC5883AHRS/blob/master/MPU6050HMC5883AHRS.ino


class SensorToDigital : public RadioClass
{
private:
    enum MenuEntries{PREASURE_AT_SEALEVEL, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;
    //Adafruit_10DOF                    dof;
    //Adafruit_LSM303_Accel_Unified     accel;
    //Adafruit_LSM303_Mag_Unified       mag;
    //Adafruit_BMP085_Unified           bmp;
   // Adafruit_MPU6050&     accel;
    //Adafruit_HMC5883_Unified&    mag;
    //Adafruit_Simple_AHRS&              ahrs;
    //Adafruit_BMP085_Unified&           bmp;
    float temperature;
    
public:
    SensorToDigital(TFT_eSPI& newTft, RadioData& newRadioData
                    //Adafruit_MPU6050& newMpu,
                    //Adafruit_HMC5883_Unified& newMag,
                    //Adafruit_BMP085_Unified& newBmp
                    ): 
                    //accel(newMpu), mag(newMag), bmp(newBmp), 
                    RadioClass(newTft, newRadioData){}
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

void SensorToDigital::showMenu()
{
    sprintf(myString,"Pre. at Seal. = %4.2fhPa\n", radioData.sensorToDigitalData.seaLevelPressure);
    tft.drawString(myString, posW, posH+incH*0);

    sprintf(myString,"Pitch = %3.2f\n", radioData.digitalData.pitch);
    tft.drawString(myString, posW, posH+incH*1);

    sprintf(myString,"Roll = %3.2f\n", radioData.digitalData.roll);
    tft.drawString(myString, posW, posH+incH*2);

    sprintf(myString,"Heading = %3.2f\n", radioData.digitalData.heading);
    tft.drawString(myString, posW, posH+incH*3);

    sprintf(myString,"Altitude = %4.2fm\n", radioData.digitalData.altitude);
    tft.drawString(myString, posW, posH+incH*4);

    //sprintf(myString,"Temperature = %2.2f\n", temperature);
    //tft.drawString(myString, posW, posH+incH*5);

    //sensor_t sensor;
    //accel.get(&sensor);
    //sprintf(myString,"%s,%d,%d\n", sensor.name, sensor.type, sensor.version);
    //tft.drawString(myString, posW, posH+incH*5);
    //mag.getSensor(&sensor);
    //sprintf(myString,"%s,%d,%d\n", sensor.name, sensor.type, sensor.version);
    //tft.drawString(myString, posW, posH+incH*6);
    //bmp.getSensor(&sensor);
    //sprintf(myString,"%s,%d,%d\n", sensor.name, sensor.type, sensor.version);
    //tft.drawString(myString, posW, posH+incH*7);

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
            radioData.sensorToDigitalData.seaLevelPressure -= 10;
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
        radioData.sensorToDigitalData.seaLevelPressure += 10;
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
/*    sensors_event_t accel_event;
    sensors_event_t mag_event;
    sensors_event_t gyro_event;
    sensors_event_t temperature_event;
    sensors_vec_t   orientation;
    sensors_event_t bmp_event;
  
    accel.getEvent(&accel_event, &gyro_event, &temperature_event);
    mag.getEvent(&mag_event);
    //if (ahrs.getOrientation(&orientation))
    //if (dof.fusionGetOrientation(&accel_event,&mag_event,&orientation))
    //{

        radioData.digitalData.roll = orientation.roll;
        radioData.digitalData.pitch = orientation.pitch;
        radioData.digitalData.heading = orientation.heading;
    //} 

    bmp.getEvent(&bmp_event);
    bmp.getTemperature(&temperature);
    radioData.digitalData.altitude =  bmp.pressureToAltitude(
                                        radioData.sensorToDigitalData.seaLevelPressure,
                                        bmp_event.pressure,
                                        temperature);
                                        */
}

void SensorToDigital::showValue()
{
    
}

#endif