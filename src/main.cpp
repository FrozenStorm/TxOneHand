/* -------------------- Include --------------------------------------------------------------------------------*/
#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
//#include "SensorToDigital.hpp"
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
#define PIN_ACCELEROMETER_SCL 17
#define PIN_ACCELEROMETER_SDA 18
#define PIN_GPS_TX            12
#define PIN_GPS_RX            13

#define PIN_POWER_EN          15
#define PIN_LCD_BL            38

#define DISP_WIDTH            170
#define DISP_HEIGHT           320

/* -------------------- Variable -------------------------------------------------------------------------------*/
TFT_eSPI                          tft = TFT_eSPI(); 
uint32_t                          targetTime = 0;         
RadioData                         radioData = RadioData();
//SensorToDigital                   sensorToDigital = SensorToDigital(tft, radioData);
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

/* -------------------- Setup ----------------------------------------------------------------------------------*/
void setup() {

  Serial.begin(115200);

  // Menu
  radioMenu.addEntry(&model);
  radioMenu.addEntry(&transmitter);
  //radioMenu.addEntry(&sensorToDigital);
  radioMenu.addEntry(&analogToDigital);
  radioMenu.addEntry(&expo);
  radioMenu.addEntry(&dualRate);
  radioMenu.addEntry(&mixer);
  radioMenu.addEntry(&functionToChannel);  

  // Power Enable (for LCD Backlight without VBUS from USB)
  pinMode(PIN_POWER_EN,OUTPUT);
  digitalWrite(PIN_POWER_EN,HIGH);
  //ledcSetup(0, 10000, 8);
  //ledcAttachPin(PIN_LCD_BL, 0);
  //ledcWrite(0, 10); // 0..255 Backlight intensity

  // Display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_WHITE,TFT_BLACK,true);  
  tft.setTextSize(1);

  //EEPROM
  // radioData.loadSelectedModel();
  // radioData.loadData();
  
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
  }
  //sensorToDigital.doFunction();
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