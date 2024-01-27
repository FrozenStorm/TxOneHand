/* -------------------- Include --------------------------------------------------------------------------------*/
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_BMP085_U.h>

/* -------------------- Defines --------------------------------------------------------------------------------*/
#define PIN_GPS_TX            12
#define PIN_GPS_RX            13

#define PIN_POWER_EN          15

#define DISP_WIDTH            170
#define DISP_HEIGHT           320

#define PIN_ACCELEROMETER_SCL 17
#define PIN_ACCELEROMETER_SDA 18

/* -------------------- Variable -------------------------------------------------------------------------------*/
Adafruit_MPU6050        mpu;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);       
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);   
/* -------------------- Functions Prototypes -------------------------------------------------------------------*/

bool errorBmp = false;
bool errorMag = false;
bool errorMpu = false;
/* -------------------- Setup ----------------------------------------------------------------------------------*/
void setup() {

  Serial.begin(115200);

  delay(4000);


  // Power Enable (for LCD Backlight without VBUS from USB)
  pinMode(PIN_POWER_EN,OUTPUT);
  digitalWrite(PIN_POWER_EN,HIGH);

  // UART
  // Serial2.begin(9600, PIN_GPS_RX, PIN_GPS_TX);

  Wire.setPins(PIN_ACCELEROMETER_SDA,PIN_ACCELEROMETER_SCL);
  Wire.begin();

  if(!mpu.begin(0b1101000,&Wire,1010)) errorMpu = true;

  mpu.setHighPassFilter(MPU6050_HIGHPASS_DISABLE);
  mpu.setMotionDetectionThreshold(0);
  mpu.setMotionDetectionDuration(1);
  mpu.setInterruptPinLatch(false);	// Keep it latched.  Will turn off when reinitialized.
  mpu.setInterruptPinPolarity(true);
  mpu.setMotionInterrupt(false);

  if(!mag.begin())
  {
    /* There was a problem detecting the HMC5883 ... check your connections */
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
    while(1);
  }

  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }

}

/* -------------------- Main -----------------------------------------------------------------------------------*/
void loop() {
  char myString[20];
  sensor_t sensor;
  sensors_event_t event;

  sprintf(myString,"Error MPU = %d",errorMpu);  
  Serial.println(myString);

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  sprintf(myString,"AccelX = %4.4f",a.acceleration.x);
  Serial.println(myString);
  sprintf(myString,"AccelY = %4.4f",a.acceleration.y);
  Serial.println(myString);
  sprintf(myString,"AccelZ = %4.4f",a.acceleration.z);
  Serial.println(myString);
  sprintf(myString,"GyroX = %4.4f",g.gyro.x);
  Serial.println(myString);
  sprintf(myString,"GyroY = %4.4f",g.gyro.y);
  Serial.println(myString);
  sprintf(myString,"GyroZ = %4.4f",g.gyro.z);
  Serial.println(myString);









  mag.getEvent(&event);
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.22;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  Serial.print("Heading (degrees): "); Serial.println(headingDegrees);










  bmp.getEvent(&event);
 
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(event.pressure);
    Serial.println(" hPa");
    
    /* Calculating altitude with reasonable accuracy requires pressure    *
     * sea level pressure for your position at the moment the data is     *
     * converted, as well as the ambient temperature in degress           *
     * celcius.  If you don't have these values, a 'generic' value of     *
     * 1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA   *
     * in sensors.h), but this isn't ideal and will give variable         *
     * results from one day to the next.                                  *
     *                                                                    *
     * You can usually find the current SLP value by looking at weather   *
     * websites or from environmental information centers near any major  *
     * airport.                                                           *
     *                                                                    *
     * For example, for Paris, France you can check the current mean      *
     * pressure and sea level at: http://bit.ly/16Au8ol                   */
     
    /* First we get the current temperature from the BMP085 */
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Serial.print("Altitude:    "); 
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure)); 
    Serial.println(" m");
    Serial.println("");
  }


  delay(500);

/*
  static uint32_t lastCall = millis();
  char myString[20];
  uint32_t deltaT = 0;
  static uint32_t maxHold = 0;

  deltaT = millis()-lastCall;
  lastCall = lastCall + deltaT;
  if(deltaT > maxHold) maxHold = deltaT;
  sprintf(myString,"%3d/%dms",deltaT, maxHold);
  Serial.setTextDatum(TR_DATUM);
  Serial.drawString(myString,Serial.width(),15,2);    
  
  if (targetTime < millis()) {
    targetTime += 100;
    radioMenu.showMenu();
    
    // gnss.serialRead();
    // Serial.printf("D,T=%s,%s\n",gnss.UTCDate.c_str(),gnss.UTCTime.c_str());
    // Serial.printf("lat=%.7f\n",gnss.lat);
    // Serial.printf("lon=%.7f\n",gnss.lon);
    // Serial.printf("vel,cou=%.2f,%.2f\n",gnss.velocity, gnss.course);

    // readSensor();
  }
  analogToDigital.doFunction();
  //sensorToDigital.doFunction();
  radioMenu.processInputs();
  digitalToFunction.doFunction();
  expo.doFunction();
  trim.doFunction();
  dualRate.doFunction();
  mixer.doFunction();
  functionToChannel.doFunction();
  transmitter.doFunction();
  */
}

/* -------------------- Functions ------------------------------------------------------------------------------*/

