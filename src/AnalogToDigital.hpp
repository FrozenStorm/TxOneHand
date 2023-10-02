#ifndef ANALOG_TO_DIGITAL_HPP
#define ANALOG_TO_DIGITAL_HPP

#include "RadioClass.hpp"

#define PIN_JOY_PITCH         1
#define PIN_JOY_ROLL          2
#define PIN_THROTTLE          3
#define PIN_BAT_VOLTAGE       4
#define PIN_MENU              10 

#define PIN_SWITCH            11  

class AnalogToDigital : public RadioClass
{
private:
    void getMenuButton(const double& value, const double& limit, const double& tolerance, bool& button, bool& buttonEvent);
    double analogToDigital(double value, const RadioData::AnalogToDigitalData::StickLimit& limit);
    void getLongPress(bool& longPressEvent, unsigned long& startTimeMs, const bool& state, const bool& event);
public:
    AnalogToDigital(TFT_eSPI& newTft, RadioData& newRadioData);
    void doFunction();
    void showValue();

    void showMenu();
    const char * getTitle();
    void up();
    void down();
    void left();
    void right();
    void center();
};

void AnalogToDigital::showMenu(){};
const char * AnalogToDigital::getTitle()
{
    return "AnalogToDigital";
}
void AnalogToDigital::up(){};
void AnalogToDigital::down(){};
void AnalogToDigital::left(){};
void AnalogToDigital::right(){};
void AnalogToDigital::center(){};

AnalogToDigital::AnalogToDigital(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData)
{
    analogSetAttenuation(ADC_11db); // 0 mV ~ 3100 mV
    //analogReadResolution(16); // Effective ADC resolution 13 bits
    adcAttachPin(PIN_JOY_ROLL);
    adcAttachPin(PIN_JOY_PITCH);
    adcAttachPin(PIN_THROTTLE);
    adcAttachPin(PIN_MENU);
    adcAttachPin(PIN_BAT_VOLTAGE);

    pinMode(PIN_SWITCH, INPUT_PULLUP);
}

void AnalogToDigital::doFunction()
{
    radioData.rawData.battery = 0;
    radioData.rawData.menu = 0;
    radioData.rawData.stickUpDown = 0;
    radioData.rawData.stickLeftRight = 0;
    radioData.rawData.slider = 0;
    for(int i=0;i<10;i++){
        radioData.rawData.battery += analogRead(PIN_BAT_VOLTAGE);
        radioData.rawData.menu += analogRead(PIN_MENU);
        radioData.rawData.stickUpDown += analogRead(PIN_JOY_PITCH);
        radioData.rawData.stickLeftRight += analogRead(PIN_JOY_ROLL);
        radioData.rawData.slider += analogRead(PIN_THROTTLE);
    }
    radioData.rawData.battery = radioData.rawData.battery/10;
    radioData.rawData.menu = radioData.rawData.menu/10;
    radioData.rawData.stickUpDown = radioData.rawData.stickUpDown/10;
    radioData.rawData.stickLeftRight = radioData.rawData.stickLeftRight/10;
    radioData.rawData.slider = radioData.rawData.slider/10;
    

    radioData.analogData.battery = radioData.rawData.battery*3.1/4096.0*2;
    radioData.analogData.menu = radioData.rawData.menu*3.1/4096.0;
    radioData.analogData.stickUpDown = radioData.rawData.stickUpDown*3.1/4096.0;
    radioData.analogData.stickLeftRight = radioData.rawData.stickLeftRight*3.1/4096.0;
    radioData.analogData.slider = radioData.rawData.slider*3.1/4096.0;

    radioData.digitalData.stickUpDown = analogToDigital(radioData.analogData.stickUpDown, radioData.analogToDigitalData.stickLimitUpDown);
    radioData.digitalData.stickLeftRight = analogToDigital(radioData.analogData.stickLeftRight, radioData.analogToDigitalData.stickLimitLeftRight);
    radioData.digitalData.slider = analogToDigital(radioData.analogData.slider, radioData.analogToDigitalData.stickLimitSlider);

    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.up, radioData.analogToDigitalData.menuButtonTolerance, radioData.digitalData.up, radioData.digitalData.upEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.down, radioData.analogToDigitalData.menuButtonTolerance, radioData.digitalData.down, radioData.digitalData.downEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.left, radioData.analogToDigitalData.menuButtonTolerance, radioData.digitalData.left, radioData.digitalData.leftEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.right, radioData.analogToDigitalData.menuButtonTolerance, radioData.digitalData.right, radioData.digitalData.rightEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.center, radioData.analogToDigitalData.menuButtonTolerance, radioData.digitalData.center, radioData.digitalData.centerEvent);
    radioData.digitalData.sideSwitch = digitalRead(PIN_SWITCH);

    getLongPress(radioData.digitalData.upLongPressEvent,radioData.analogToDigitalData.startPressTimeUpMs,radioData.digitalData.up,radioData.digitalData.upEvent);
    getLongPress(radioData.digitalData.downLongPressEvent,radioData.analogToDigitalData.startPressTimeDownMs,radioData.digitalData.down,radioData.digitalData.downEvent);
    getLongPress(radioData.digitalData.leftLongPressEvent,radioData.analogToDigitalData.startPressTimeLeftMs,radioData.digitalData.left,radioData.digitalData.leftEvent);
    getLongPress(radioData.digitalData.rightLongPressEvent,radioData.analogToDigitalData.startPressTimeRightMs,radioData.digitalData.right,radioData.digitalData.rightEvent);
    getLongPress(radioData.digitalData.centerLongPressEvent,radioData.analogToDigitalData.startPressTimeCenterMs,radioData.digitalData.center,radioData.digitalData.centerEvent);
}

void AnalogToDigital::showValue()
{
    tft.print("PIN_BAT_VOLTAGE=");
    tft.println(radioData.analogData.battery);
    //tft.print("PIN_MENU=");
    //tft.println(radioData.analogData.menu);
    //tft.print("PIN_THROTTLE=");
    //tft.println(radioData.digitalData.slider);
    //tft.print("PIN_JOY_ROLL=");
    //tft.println(radioData.digitalData.stickLeftRight);
    //tft.print("PIN_JOY_PITCH=");
    //tft.println(radioData.digitalData.stickUpDown);
    tft.print("PIN_SWITCH=");
    tft.println(radioData.digitalData.sideSwitch);
    tft.print("Value udlrc=");
    tft.print(radioData.digitalData.up);
    tft.print(radioData.digitalData.down);
    tft.print(radioData.digitalData.left);
    tft.print(radioData.digitalData.right);
    tft.println(radioData.digitalData.center);
}

void AnalogToDigital::getMenuButton(const double& value, const double& limit, const double& tolerance, bool& button, bool& buttonEvent)
{
    bool newButtonState;
    if(value <= limit + tolerance && value >= limit - tolerance) newButtonState = true;
    else newButtonState = false;

    if(button == false && newButtonState == true) buttonEvent = true;
    else buttonEvent = false;

    button = newButtonState;
}

void AnalogToDigital::getLongPress(bool& longPressEvent, unsigned long& startTimeMs, const bool& state, const bool& event)
{
    if(event == true) 
    {
        startTimeMs = millis();
    }
    if(state == true && ((double)millis() - (double)startTimeMs) >= radioData.analogToDigitalData.longPressDurationMs)
    {
        longPressEvent = true;
        startTimeMs = millis() + 60000;       
    }
    else 
    {
        longPressEvent = false;
    }
}

double AnalogToDigital::analogToDigital(double value, const RadioData::AnalogToDigitalData::StickLimit& limit)
{
    // Umwandeln aller Werte in +/- Werte
    double max = limit.max - limit.center;
    double min = limit.min - limit.center;
    value = value - limit.center;
    // Umwandeln von analog Bereich zu +/- 1
    if(value >= 0){
        if(max != 0){
            value = value / max;
        }
        else{
            tft.println("Error /0");
        }
    }
    else{
        if(min != 0){
            value = value / abs(min);
        }
        else{
            tft.println("Error /0");
        }
    }
    // Invertieren
    if(limit.invert == true){
        value = value * -1;
    }
    // Limitieren auf digital Bereich
    limitValue(value);

    return value;
}

#endif