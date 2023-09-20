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
#include "AnalogInputs.hpp"
#include "DigitalValues.hpp"
#include "Expo.hpp"
#include "Trim.hpp"
#include "RadioMenu.hpp"

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
char                              txData[27] = {0x55,0x06,0x20,0x07,0x00,0x24,0x20,0x07,0x01,0x08,0x40,0x00,0x02,0x10,0x80,0x00,0x04,0x20,0x00,0x01,0x08,0x40,0x00,0x02,0x10,0x80,0x08};
DigitalValues                     digitalValues = DigitalValues();
AnalogInputs                      analogInputs = AnalogInputs(tft, digitalValues);
Expo                              expo = Expo(tft, digitalValues);
Trim                              trim = Trim(tft, digitalValues);

//RadioMenu                              menu = RadioMenu(trim, digitalValues);


/* -------------------- Functions Prototypes -------------------------------------------------------------------*/
void readSensor(void);
void sendTxData(void);

/* -------------------- Setup ----------------------------------------------------------------------------------*/
void setup() {

  Serial.begin(115200);

  // Display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);  
  tft.setTextSize(1);

  tft.println("TxOneHand by Z-Craft");

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

  // GPIO
  

  // Power Enable (for LCD Backlight without VBUS from USB)
  pinMode(PIN_POWER_EN,OUTPUT);
  digitalWrite(PIN_POWER_EN,HIGH);

  // UART
  Serial1.begin(100000, SERIAL_8E2, PIN_MULTI_RX, PIN_MULTI_TX);
  Serial2.begin(9600, PIN_GPS_RX, PIN_GPS_TX);

  //GPS
  gnss.init(Serial2, 9600);
  
  // Loop Delay
  targetTime = millis() + 1000; 
}

/* -------------------- Main -----------------------------------------------------------------------------------*/
void loop() {
  // put your main code here, to run repeatedly:
  
  if (targetTime < millis()) {
    targetTime += 100;

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(15, 140, 2);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);  
    tft.setTextSize(1);

    tft.println("TxOneHand by Z-Craft");
    tft.println();
    analogInputs.showValue();
    // menu.showMenu();
    trim.showMenu();
    

    gnss.serialRead();
    tft.printf("D,T=%s,%s\n",gnss.UTCDate.c_str(),gnss.UTCTime.c_str());
    tft.printf("lat=%.7f\n",gnss.lat);
    tft.printf("lon=%.7f\n",gnss.lon);
    tft.printf("vel,cou=%.2f,%.2f\n",gnss.velocity, gnss.course);

    // readSensor();
  }
  analogInputs.doFunction();
  // menu.processInputs();
  if(digitalValues.upEvent == true) trim.up();
  if(digitalValues.downEvent == true) trim.down();
  if(digitalValues.leftEvent == true) trim.left();
  if(digitalValues.rightEvent == true) trim.right();
  if(digitalValues.centerEvent == true) trim.center();
  expo.doFunction();
  trim.doFunction();
  sendTxData();

  delay(10);
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

void sendTxData(void){
  int channel[16];
  double mix0 = (-digitalValues.stickLeftRight - digitalValues.stickUpDown);
  if(mix0 > 1) mix0 = 1;
  if(mix0 < -1) mix0 = -1;
  double mix1 = (-digitalValues.stickLeftRight + digitalValues.stickUpDown);
  if(mix1 > 1) mix1 = 1;
  if(mix1 < -1) mix1 = -1;
  channel[0] = (mix0 + 1) * ((1<<10)-1);
  channel[1] = (mix1 + 1) * ((1<<10)-1);
  channel[2] = (digitalValues.slider + 1) * ((1<<10)-1);
  channel[13] = channel[2];
  for(int i = 0; i < 16; i++){
    if(channel[i] > ((1<<11)-1)) channel[i] = ((1<<11)-1);
    if(channel[i] < 0) channel[i] = 0;
//    Serial.print("ch");
//    Serial.print(i);
//    Serial.print(", value=");
//    Serial.print(channel[i]);
//    Serial.println("");
  }
  
  for(int i = 0; i < 16*11; i++){
    if(channel[i/11] & (0x01 << (i % 11))){
      txData[4+i/8] |= (0x01 << (i % 8));
    }
    else{
      txData[4+i/8] &= ~(0x01 << (i % 8));
    }
  }
  Serial1.write(txData,27);
}