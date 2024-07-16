#ifndef SENSOR_TO_DIGITAL_HPP
#define SENSOR_TO_DIGITAL_HPP

#include "RadioClass.hpp"

#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_10DOF.h>
//#include <Adafruit_LSM303_U.h>
//#include <Adafruit_HMC5883_U.h>
//#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP085.h>

#define PIN_ACCELEROMETER_SCL 17
#define PIN_ACCELEROMETER_SDA 18


class SensorToDigital : public RadioClass
{
private:
    enum MenuEntries{PREASURE_AT_SEALEVEL, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;
    //Adafruit_10DOF                    dof   = Adafruit_10DOF();
    //Adafruit_LSM303_Accel_Unified     accel = Adafruit_LSM303_Accel_Unified(30301);
    //Adafruit_LSM303_Mag_Unified       mag   = Adafruit_LSM303_Mag_Unified(30302);
    Adafruit_BMP085_Unified           bmp   = Adafruit_BMP085_Unified(18001);
    //Adafruit_MPU6050     accel;
    //Adafruit_HMC5883_Unified&    mag;
    //Adafruit_Simple_AHRS&              ahrs;
    //Adafruit_BMP085_Unified&           bmp;
    float temperature;
    
public:
    SensorToDigital(TFT_eSPI& newTft, RadioData& newRadioData);
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

SensorToDigital::SensorToDigital(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData)
{
  // I2C 10DOF Sensor
  Wire.setPins(PIN_ACCELEROMETER_SDA,PIN_ACCELEROMETER_SCL);
  Wire.begin();


  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    //Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    //while(1);
  }
}

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

    sprintf(myString,"Temperature = %2.2f\n", temperature);
    tft.drawString(myString, posW, posH+incH*5);

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
    sensors_event_t bmp_event;

    bmp.getEvent(&bmp_event);
    bmp.getTemperature(&temperature);
    radioData.digitalData.altitude =  bmp.pressureToAltitude(
                                        radioData.sensorToDigitalData.seaLevelPressure,
                                        bmp_event.pressure,
                                        temperature);
                                        
/*    sensors_event_t accel_event;
    sensors_event_t mag_event;
    sensors_event_t gyro_event;
    sensors_event_t temperature_event;
    sensors_vec_t   orientation;
    
  
    accel.getEvent(&accel_event, &gyro_event, &temperature_event);
    mag.getEvent(&mag_event);
    //if (ahrs.getOrientation(&orientation))
    //if (dof.fusionGetOrientation(&accel_event,&mag_event,&orientation))
    //{

        radioData.digitalData.roll = orientation.roll;
        radioData.digitalData.pitch = orientation.pitch;
        radioData.digitalData.heading = orientation.heading;
    //} 
*/
    
}

void SensorToDigital::showValue()
{
    
}

#endif