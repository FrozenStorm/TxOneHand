#ifndef ANALOG_TO_DIGITAL_HPP
#define ANALOG_TO_DIGITAL_HPP

#include "RadioClass.hpp"

#define PIN_JOY_PITCH           1
#define PIN_JOY_ROLL            2
#define PIN_THROTTLE            3
#define PIN_BAT_VOLTAGE         4
#define PIN_MENU                10 

#define PIN_SIDE_SWITCH         11  

#define BATTERY_VOLTAGE_OFFSET  0.4

#define ADC_REF_VOLTAGE         3.1
#define ADC_MAX_VALUE           4096.0
#define FILTER_LENGTH           10

class AnalogToDigital : public RadioClass
{
private:
    unsigned int startPressTimeUpMs = 0;
    unsigned int startPressTimeDownMs = 0;
    unsigned int startPressTimeLeftMs = 0;
    unsigned int startPressTimeRightMs = 0;
    unsigned int startPressTimeCenterMs = 0;
    unsigned int changeTimeUpMs = 0;
    unsigned int changeTimeDownMs = 0;
    unsigned int changeTimeLeftMs = 0;
    unsigned int changeTimeRightMs = 0;
    unsigned int changeTimeCenterMs = 0;
    void getMenuButton(const float& value, const float& limit, const float& tolerance, unsigned int& changeTimeMs, bool& button, bool& buttonEvent);
    float analogToDigital(float value, const RadioData::AnalogToDigitalData::StickLimit& limit);
    void getLongPress(bool& longPressEvent, unsigned int& startTimeMs, const bool& state, const bool& event);
public:
    AnalogToDigital(RadioData& newRadioData);
    void doFunction();
};

AnalogToDigital::AnalogToDigital(RadioData& newRadioData) : RadioClass(newRadioData)
{
    analogSetAttenuation(ADC_11db); // 0 mV ~ 3100 mV
    //analogReadResolution(16); // Effective ADC resolution 13 bits
    adcAttachPin(PIN_JOY_ROLL);
    adcAttachPin(PIN_JOY_PITCH);
    adcAttachPin(PIN_THROTTLE);
    adcAttachPin(PIN_MENU);
    adcAttachPin(PIN_BAT_VOLTAGE);
    adcAttachPin(PIN_SIDE_SWITCH);
}

void AnalogToDigital::doFunction()
{
    radioData.rawData.menu = analogRead(PIN_MENU);
    radioData.rawData.sideSwitch = analogRead(PIN_SIDE_SWITCH);

    radioData.rawData.stickUpDown = 0;
    radioData.rawData.stickLeftRight = 0;
    radioData.rawData.slider = 0;
    radioData.rawData.battery = 0;
    for(int i=0;i<FILTER_LENGTH;i++){
        radioData.rawData.battery += analogRead(PIN_BAT_VOLTAGE);
        radioData.rawData.stickUpDown += analogRead(PIN_JOY_PITCH);
        radioData.rawData.stickLeftRight += analogRead(PIN_JOY_ROLL);
        radioData.rawData.slider += analogRead(PIN_THROTTLE);
    }
    radioData.rawData.battery = radioData.rawData.battery/FILTER_LENGTH;    
    radioData.rawData.stickUpDown = radioData.rawData.stickUpDown/FILTER_LENGTH;
    radioData.rawData.stickLeftRight = radioData.rawData.stickLeftRight/FILTER_LENGTH;
    radioData.rawData.slider = radioData.rawData.slider/FILTER_LENGTH;

    radioData.analogData.battery = radioData.rawData.battery*ADC_REF_VOLTAGE/ADC_MAX_VALUE*2+BATTERY_VOLTAGE_OFFSET;
    radioData.analogData.menu = radioData.rawData.menu*ADC_REF_VOLTAGE/ADC_MAX_VALUE; 
    radioData.analogData.stickUpDown = radioData.rawData.stickUpDown*ADC_REF_VOLTAGE/ADC_MAX_VALUE;
    radioData.analogData.stickLeftRight = radioData.rawData.stickLeftRight*ADC_REF_VOLTAGE/ADC_MAX_VALUE;
    radioData.analogData.slider = radioData.rawData.slider*ADC_REF_VOLTAGE/ADC_MAX_VALUE;
    radioData.analogData.sideSwitch = radioData.rawData.sideSwitch*ADC_REF_VOLTAGE/ADC_MAX_VALUE;

    radioData.digitalData.stickUpDown = analogToDigital(radioData.analogData.stickUpDown, radioData.analogToDigitalData.stickLimitUpDown);
    radioData.digitalData.stickLeftRight = analogToDigital(radioData.analogData.stickLeftRight, radioData.analogToDigitalData.stickLimitLeftRight);
    radioData.digitalData.slider = analogToDigital(radioData.analogData.slider, radioData.analogToDigitalData.stickLimitSlider);

    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.up, radioData.analogToDigitalData.menuButtonTolerance, changeTimeUpMs, radioData.digitalData.up, radioData.digitalData.upEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.down, radioData.analogToDigitalData.menuButtonTolerance, changeTimeDownMs, radioData.digitalData.down, radioData.digitalData.downEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.left, radioData.analogToDigitalData.menuButtonTolerance, changeTimeLeftMs, radioData.digitalData.left, radioData.digitalData.leftEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.right, radioData.analogToDigitalData.menuButtonTolerance, changeTimeRightMs, radioData.digitalData.right, radioData.digitalData.rightEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.center, radioData.analogToDigitalData.menuButtonTolerance, changeTimeCenterMs, radioData.digitalData.center, radioData.digitalData.centerEvent);
    
    radioData.digitalData.sideSwitchEvent = false;
    if(radioData.analogData.sideSwitch > 0.75 * ADC_REF_VOLTAGE) 
    {
        if(radioData.digitalData.sideSwitch != 2)
        {
            radioData.digitalData.sideSwitch = 2;
            radioData.digitalData.sideSwitchEvent = true;
        }
    }
    else if(radioData.analogData.sideSwitch < 0.25 * ADC_REF_VOLTAGE) 
    {
        if(radioData.digitalData.sideSwitch != 1)
        {
            radioData.digitalData.sideSwitch = 1;
            radioData.digitalData.sideSwitchEvent = true;
        }
    }
    else 
    {
        if(radioData.digitalData.sideSwitch != 0)
        {
            radioData.digitalData.sideSwitch = 0;
            radioData.digitalData.sideSwitchEvent = true;
        }
    }

    getLongPress(radioData.digitalData.upLongPressEvent,startPressTimeUpMs,radioData.digitalData.up,radioData.digitalData.upEvent);
    getLongPress(radioData.digitalData.downLongPressEvent,startPressTimeDownMs,radioData.digitalData.down,radioData.digitalData.downEvent);
    getLongPress(radioData.digitalData.leftLongPressEvent,startPressTimeLeftMs,radioData.digitalData.left,radioData.digitalData.leftEvent);
    getLongPress(radioData.digitalData.rightLongPressEvent,startPressTimeRightMs,radioData.digitalData.right,radioData.digitalData.rightEvent);
    getLongPress(radioData.digitalData.centerLongPressEvent,startPressTimeCenterMs,radioData.digitalData.center,radioData.digitalData.centerEvent);
}

void AnalogToDigital::getMenuButton(const float& value, const float& limit, const float& tolerance, unsigned int& changeTimeMs, bool& button, bool& buttonEvent)
{
    bool newButtonState;
    if(value <= limit + tolerance && value >= limit - tolerance) 
    {
        newButtonState = true;
        if(changeTimeMs == 0)
        {
            changeTimeMs = millis();
        }
    }
    else 
    {
        newButtonState = false;
        changeTimeMs = 0;
    }

    if(button == false && newButtonState == true)
    {
        if(millis() - changeTimeMs >= radioData.analogToDigitalData.longPressDurationMs/10)
        {
            buttonEvent = true;
            button = newButtonState;
        }
    }
    else 
    {
        buttonEvent = false;
        button = newButtonState;
    }
}

void AnalogToDigital::getLongPress(bool& longPressEvent, unsigned int& startTimeMs, const bool& state, const bool& event)
{
    if(event == true) 
    {
        startTimeMs = millis();
    }
    if(state == true && ((float)millis() - (float)startTimeMs) >= radioData.analogToDigitalData.longPressDurationMs)
    {
        longPressEvent = true;
        startTimeMs = millis() + 60000;       
    }
    else 
    {
        longPressEvent = false;
    }
}

float AnalogToDigital::analogToDigital(float value, const RadioData::AnalogToDigitalData::StickLimit& limit)
{
    // Umwandeln aller Werte in +/- Werte
    float max = limit.max - limit.center;
    float min = limit.min - limit.center;
    value = value - limit.center;
    // Umwandeln von analog Bereich zu +/- 1
    if(value >= 0){
        if(max != 0){
            value = value / max;
        }
        else{

        }
    }
    else{
        if(min != 0){
            value = value / abs(min);
        }
        else{

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