/* -------------------- Include --------------------------------------------------------------------------------*/
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "AnalogToDigital.hpp"
#include "DigitalToFunction.hpp"
#include "Expo.hpp"
#include "Trim.hpp"
#include "Mixer.hpp"
#include "FunctionToChannel.hpp"
#include "Transmitter.hpp"
#include "Model.hpp"
#include "DualRate.hpp"
#include "SensorToDigital.hpp"
#include <nvs_flash.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP085.h>

/* -------------------- Defines --------------------------------------------------------------------------------*/
#define PIN_ACCELEROMETER_SCL 16
#define PIN_ACCELEROMETER_SDA 18
#define PIN_GPS_TX            12
#define PIN_GPS_RX            13

#define PIN_POWER_EN          15
#define PIN_LCD_BL            38

#define DISP_WIDTH            170
#define DISP_HEIGHT           320

#define LOOP_DELAY_MS         20

/* -------------------- Variable -------------------------------------------------------------------------------*/
Adafruit_MPU6050                  mpu;
Adafruit_BMP085                   bmp;
uint32_t                          targetTime = 0;         
RadioData                         radioData = RadioData();
AnalogToDigital                   analogToDigital = AnalogToDigital(radioData);
DigitalToFunction                 digitalToFunction = DigitalToFunction(radioData);
Expo                              expo = Expo(radioData);
DualRate                          dualRate = DualRate(radioData);
Trim                              trim = Trim(radioData);
Mixer                             mixer = Mixer(radioData);
FunctionToChannel                 functionToChannel = FunctionToChannel(radioData);
Transmitter                       transmitter = Transmitter(radioData);
Model                             model = Model(radioData);
SensorToDigital                   sensorToDigital = SensorToDigital(radioData, &mpu, &bmp);
/* -------------------- Functions Prototypes -------------------------------------------------------------------*/

/* -------------------- Setup ----------------------------------------------------------------------------------*/
void setup() {
  Serial.begin(115200);
  Serial.println("Init started");
  // Factory Reset FLASH
  //nvs_flash_erase();      // erase the NVS partition and...
  //nvs_flash_init();       // initialize the NVS partition.

  // Load Models
  radioData.loadGlobalData();
  radioData.loadModelData();
  //radioData.storeGlobalData();
  //radioData.storeModelData();
  Serial.println("Model loaded");


  // MPU6050 Senosr Board
  Wire.begin(PIN_ACCELEROMETER_SDA,PIN_ACCELEROMETER_SCL);
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  bmp.begin();
  Serial.println("Sensor ready");
  
  // Loop Delay
  targetTime = millis() + LOOP_DELAY_MS; 
  Serial.println("Init done");
}

/* -------------------- Main -----------------------------------------------------------------------------------*/
void loop() { // Core 1
  static int slowDown = 0;
  if (targetTime < millis()) {    
    targetTime += LOOP_DELAY_MS;

    analogToDigital.doFunction(); // 3ms
    sensorToDigital.doFunction(); // 4ms
    digitalToFunction.doFunction(); // <<1ms
    expo.doFunction(); // <<1ms
    trim.doFunction(); // <<1ms
    dualRate.doFunction(); // <<1ms
    mixer.doFunction(); // <<1ms
    functionToChannel.doFunction(); // <<1ms
    transmitter.doFunction(); // <<1ms


    slowDown +=1;
    if(slowDown % 20 == 0)
    {
      Serial.println("-----------------------------------------------");
      Serial.print("radioData.functionData.pitch = "); Serial.println(radioData.functionData.pitch);
      Serial.print("radioData.functionData.roll = "); Serial.println(radioData.functionData.roll);
      Serial.print("radioData.functionData.armed = "); Serial.println(radioData.functionData.armed);
      Serial.print("radioData.analogData.battery = "); Serial.println(radioData.analogData.battery);
      Serial.print("radioData.digitalData.stickLeftRight = "); Serial.println(radioData.digitalData.stickLeftRight);
      Serial.print("radioData.digitalData.stickUpDown = "); Serial.println(radioData.digitalData.stickUpDown);
      Serial.print("radioData.digitalData.arm = "); Serial.println(radioData.digitalData.arm);
      Serial.print("radioData.digitalData.trim = "); Serial.println(radioData.digitalData.trim);
      Serial.print("radioData.digitalData.pitch = "); Serial.println(radioData.digitalData.pitch);
      Serial.print("radioData.digitalData.roll = "); Serial.println(radioData.digitalData.roll);
    }
  }
}