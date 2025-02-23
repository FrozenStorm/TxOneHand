/* -------------------- Include --------------------------------------------------------------------------------*/
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
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
#include "SensorToDigital.hpp"
#include <nvs_flash.h>

/* -------------------- Defines --------------------------------------------------------------------------------*/
#define PIN_ACCELEROMETER_SCL 17
#define PIN_ACCELEROMETER_SDA 18
#define PIN_GPS_TX            12
#define PIN_GPS_RX            13

#define PIN_POWER_EN          15
#define PIN_LCD_BL            38

#define DISP_WIDTH            170
#define DISP_HEIGHT           320

#define LOOP_DELAY_MS         20

/* -------------------- Variable -------------------------------------------------------------------------------*/
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
SensorToDigital*                  sensorToDigital;
/* -------------------- Functions Prototypes -------------------------------------------------------------------*/

/* -------------------- Setup ----------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);
  Serial.println("Init started");

  sensorToDigital = new SensorToDigital(tft, radioData);
  // Factory Reset FLASH
  //nvs_flash_erase();      // erase the NVS partition and...
  //nvs_flash_init();       // initialize the NVS partition.

  // Load Models
  radioData.loadGlobalData();
  radioData.loadModelData();
  //radioData.storeGlobalData();
  //radioData.storeModelData();
  Serial.println("Model loaded");

  // Menu
  radioMenu.addEntry(&model);
  radioMenu.addEntry(&transmitter);
  radioMenu.addEntry(&analogToDigital);
  radioMenu.addEntry(sensorToDigital);
  radioMenu.addEntry(&expo);
  radioMenu.addEntry(&dualRate);
  radioMenu.addEntry(&mixer);
  radioMenu.addEntry(&functionToChannel);  
  Serial.println("Menu created");

  // Power Enable (for LCD Backlight without VBUS from USB)
  pinMode(PIN_POWER_EN,OUTPUT);
  digitalWrite(PIN_POWER_EN,HIGH);
  //ledcSetup(0, 10000, 8);
  //ledcAttachPin(PIN_LCD_BL, 0);
  //ledcWrite(0, 10); // 0..255 Backlight intensity

  // Display

  Serial.println("Display ready");

  // MPU6050 Senosr Board
  Serial.println("Sensor ready");
  
  // Loop Delay
  targetTime = millis() + LOOP_DELAY_MS; 
  Serial.println("Init done");
}

/* -------------------- Main -----------------------------------------------------------------------------------*/
void loop() {
  uint32_t startTime = millis();
  char myString[20];
  static uint32_t deltaTime = 0;
  static uint32_t maxHold = 0;
  
  if (targetTime < millis()) {    
    targetTime += LOOP_DELAY_MS;
    
    radioMenu.showMenu(); // 6ms 
    sprintf(myString,"%3d,%3d/%3d", deltaTime, maxHold, LOOP_DELAY_MS-deltaTime);
    //tft.setTextDatum(TC_DATUM);
    //tft.drawString(myString,tft.width()/2,0,2); 

    analogToDigital.doFunction(); // 3ms
    radioMenu.processInputs(); // <<1ms
    sensorToDigital->doFunction(); // 4ms
    digitalToFunction.doFunction(); // <<1ms
    expo.doFunction(); // <<1ms
    trim.doFunction(); // <<1ms
    dualRate.doFunction(); // <<1ms
    mixer.doFunction(); // <<1ms
    functionToChannel.doFunction(); // <<1ms
    transmitter.doFunction(); // <<1ms

    deltaTime = millis()-startTime;
    if(deltaTime > maxHold) maxHold = deltaTime;
  }
}