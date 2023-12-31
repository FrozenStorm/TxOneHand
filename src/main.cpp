/* -------------------- Include --------------------------------------------------------------------------------*/
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_10DOF.h>
#include <GNSS.h>
#include "AnalogToDigital.hpp"
#include "DigitalToFunction.hpp"
#include "Expo.hpp"
#include "Trim.hpp"
#include "Mixer.hpp"
#include "FunctionToChannel.hpp"
#include "Transmitter.hpp"
#include "Model.hpp"
#include "RadioMenu.hpp"
#include "DualRate.hpp"

/* -------------------- Defines --------------------------------------------------------------------------------*/
#define PIN_MULTI_TX          43
#define PIN_MULTI_RX          44
#define PIN_ACCELEROMETER_SCL 17
#define PIN_ACCELEROMETER_SDA 18
#define PIN_GPS_TX            12
#define PIN_GPS_RX            13

#define PIN_POWER_EN          15

#define DISP_WIDTH            170
#define DISP_HEIGHT           320

/* -------------------- Variable -------------------------------------------------------------------------------*/
TFT_eSPI                          tft = TFT_eSPI(); 
Adafruit_10DOF                    dof   = Adafruit_10DOF();
Adafruit_LSM303_Accel_Unified     accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified       mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_BMP085_Unified           bmp   = Adafruit_BMP085_Unified(18001);
float                             seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
GNSS                              gnss;    
uint32_t                          targetTime = 0;         
RadioData                         radioData = RadioData();
AnalogToDigital                   analogToDigital = AnalogToDigital(tft, radioData);
DigitalToFunction                 digitalToFunction = DigitalToFunction(tft, radioData);
Expo                              expo = Expo(tft, radioData);
DualRate                          dualRate = DualRate(tft, radioData);
Trim                              trim = Trim(tft, radioData);
Mixer                             mixer = Mixer(tft, radioData);
FunctionToChannel                 functionToChannel = FunctionToChannel(tft, radioData);
Transmitter                       transmitter = Transmitter(tft, radioData);
Model                             model = Model(tft, radioData);
RadioMenu                         radioMenu = RadioMenu(tft, radioData, &trim);
/* -------------------- Functions Prototypes -------------------------------------------------------------------*/
void readSensor(void);

/* -------------------- Setup ----------------------------------------------------------------------------------*/
void setup() {

  Serial.begin(115200);

  // Menu
  radioMenu.addEntry(&model);
  radioMenu.addEntry(&transmitter);
  radioMenu.addEntry(&analogToDigital);
  radioMenu.addEntry(&expo);
  radioMenu.addEntry(&dualRate);
  radioMenu.addEntry(&mixer);
  radioMenu.addEntry(&functionToChannel);  

  // Display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_WHITE,TFT_BLACK,true);  
  tft.setTextSize(1);

  // I2C 10DOF Sensor
  Wire.setPins(PIN_ACCELEROMETER_SDA,PIN_ACCELEROMETER_SCL);
  Wire.begin();
  if(!accel.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    //Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    //while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    //Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    //while(1);
  }
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP180 ... check your connections */
    //Serial.println("Ooops, no BMP180 detected ... Check your wiring!");
    //while(1);
  }

  // Power Enable (for LCD Backlight without VBUS from USB)
  pinMode(PIN_POWER_EN,OUTPUT);
  digitalWrite(PIN_POWER_EN,HIGH);

  // UART
  Serial1.begin(100000, SERIAL_8E2, -1, PIN_MULTI_TX);
  Serial2.begin(100000, SERIAL_8E2, PIN_MULTI_RX, -1, true);
  Serial2.setTimeout(4);
  // Serial2.begin(9600, PIN_GPS_RX, PIN_GPS_TX);

  //GPS
  // gnss.init(Serial2, 9600);

  //EEPROM
  radioData.loadSelectedModel();
  radioData.loadData();
  
  // Loop Delay
  targetTime = millis() + 1000; 

}

/* -------------------- Main -----------------------------------------------------------------------------------*/
void loop() {
  static uint32_t lastCall = millis();
  char myString[20];
  uint32_t deltaT = 0;
  static uint32_t maxHold = 0;

  deltaT = millis()-lastCall;
  lastCall = lastCall + deltaT;
  if(deltaT > maxHold) maxHold = deltaT;
  sprintf(myString,"%3d/%dms",deltaT, maxHold);
  tft.setTextDatum(TR_DATUM);
  tft.drawString(myString,tft.width(),15,2);    
  
  if (targetTime < millis()) {
    targetTime += 100;
    radioMenu.showMenu();
    
    // gnss.serialRead();
    // tft.printf("D,T=%s,%s\n",gnss.UTCDate.c_str(),gnss.UTCTime.c_str());
    // tft.printf("lat=%.7f\n",gnss.lat);
    // tft.printf("lon=%.7f\n",gnss.lon);
    // tft.printf("vel,cou=%.2f,%.2f\n",gnss.velocity, gnss.course);

    // readSensor();
  }
  analogToDigital.doFunction();
  radioMenu.processInputs();
  digitalToFunction.doFunction();
  expo.doFunction();
  trim.doFunction();
  dualRate.doFunction();
  mixer.doFunction();
  functionToChannel.doFunction();
  transmitter.doFunction();
}

/* -------------------- Functions ------------------------------------------------------------------------------*/
void readSensor(void){
  sensors_event_t accel_event;
  sensors_event_t mag_event;
  sensors_event_t bmp_event;
  sensors_vec_t   orientation;
  
  /* Calculate pitch and roll from the raw accelerometer data */
  accel.getEvent(&accel_event);
  if (dof.accelGetOrientation(&accel_event, &orientation))
  {
    /* 'orientation' should have valid .roll and .pitch fields */
    tft.print(F("Roll="));
    tft.println(orientation.roll);
    tft.print(F("Pitch="));
    tft.println(orientation.pitch);
  }
  
  /* Calculate the heading using the magnetometer */
  mag.getEvent(&mag_event);
  if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation))
  {
    /* 'orientation' should have valid .heading data now */
    tft.print(F("Heading="));
    tft.println(orientation.heading);
  }
  
  /* Calculate the altitude using the barometric pressure sensor */
  bmp.getEvent(&bmp_event);
  if (bmp_event.pressure)
  {
    /* Get ambient temperature in C */
    float temperature;
    bmp.getTemperature(&temperature);
    /* Convert atmospheric pressure, SLP and temp to altitude    */
    tft.print(F("Alt="));
    tft.println(bmp.pressureToAltitude(seaLevelPressure,
                                        bmp_event.pressure,
                                        temperature)); 
    /* Display the temperature */
    tft.print(F("Temp="));
    tft.println(temperature);
  }
}