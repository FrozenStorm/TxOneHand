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
    enum MenuEntries{INV_UP_DOWN, INV_LEFT_RIGHT, INV_SLIDER, LIM_UP_DOWN, LIM_LEFT_RIGHT, LIM_SLIDER, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;
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
    AnalogToDigital(TFT_eSPI& newTft, RadioData& newRadioData);
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

void AnalogToDigital::showMenu()
{
    sprintf(myString,"INV_UD : %d      %1.2fV\n", radioData.analogToDigitalData.stickLimitUpDown.invert, radioData.analogData.stickUpDown);
    tft.drawString(myString, posW, posH+incH*0);
    sprintf(myString,"INV_LR : %d      %1.2fV\n", radioData.analogToDigitalData.stickLimitLeftRight.invert, radioData.analogData.stickLeftRight);
    tft.drawString(myString, posW, posH+incH*1);
    sprintf(myString,"INV_SL : %d      %1.2fV\n", radioData.analogToDigitalData.stickLimitSlider.invert, radioData.analogData.slider);
    tft.drawString(myString, posW, posH+incH*2);

    sprintf(myString,"UD: %1.2fV:%1.2fV:%1.2fV\n", radioData.analogToDigitalData.stickLimitUpDown.min, radioData.analogToDigitalData.stickLimitUpDown.center, radioData.analogToDigitalData.stickLimitUpDown.max);
    tft.drawString(myString, posW, posH+incH*3);
    sprintf(myString,"LR: %1.2fV:%1.2fV:%1.2fV\n", radioData.analogToDigitalData.stickLimitLeftRight.min, radioData.analogToDigitalData.stickLimitLeftRight.center, radioData.analogToDigitalData.stickLimitLeftRight.max);
    tft.drawString(myString, posW, posH+incH*4);
    sprintf(myString,"SL: %1.2fV:%1.2fV:%1.2fV\n", radioData.analogToDigitalData.stickLimitSlider.min, radioData.analogToDigitalData.stickLimitSlider.center, radioData.analogToDigitalData.stickLimitSlider.max);
    tft.drawString(myString, posW, posH+incH*5);

    drawMenuPointer(selectedMenuEntry,NUMBER_OF_MENUENTRIES);
}
const char * AnalogToDigital::getTitle()
{
    return "A-To-D";
}
void AnalogToDigital::up()
{
    if(selectedMenuEntry > 0) selectedMenuEntry=(MenuEntries)(selectedMenuEntry-1);
    else selectedMenuEntry = NUMBER_OF_MENUENTRIES;
}
void AnalogToDigital::down()
{
    if(selectedMenuEntry < NUMBER_OF_MENUENTRIES) selectedMenuEntry=(MenuEntries)(selectedMenuEntry+1);
    else selectedMenuEntry = (MenuEntries)0;
}
bool AnalogToDigital::left()
{
    switch (selectedMenuEntry)
    {
    case INV_UP_DOWN:
        radioData.analogToDigitalData.stickLimitUpDown.invert = !radioData.analogToDigitalData.stickLimitUpDown.invert;
        break;
    case INV_LEFT_RIGHT:
        radioData.analogToDigitalData.stickLimitLeftRight.invert = !radioData.analogToDigitalData.stickLimitLeftRight.invert;
        break;
    case INV_SLIDER:
        radioData.analogToDigitalData.stickLimitSlider.invert = !radioData.analogToDigitalData.stickLimitSlider.invert;
        break;
    case LIM_UP_DOWN:
        radioData.analogToDigitalData.stickLimitUpDown.min = radioData.analogData.stickUpDown;
        break;
    case LIM_LEFT_RIGHT:
        radioData.analogToDigitalData.stickLimitLeftRight.min = radioData.analogData.stickLeftRight;
        break;
    case LIM_SLIDER:
        radioData.analogToDigitalData.stickLimitSlider.min = radioData.analogData.slider;
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}
bool AnalogToDigital::right()
{
    switch (selectedMenuEntry)
    {
    case LIM_LEFT_RIGHT:
        radioData.analogToDigitalData.stickLimitLeftRight.max = radioData.analogData.stickLeftRight;
        break;
    case LIM_UP_DOWN:
        radioData.analogToDigitalData.stickLimitUpDown.max = radioData.analogData.stickUpDown;
        break;
    case LIM_SLIDER:
        radioData.analogToDigitalData.stickLimitSlider.max = radioData.analogData.slider;
        break;
    case INV_LEFT_RIGHT:
        radioData.analogToDigitalData.stickLimitLeftRight.invert = !radioData.analogToDigitalData.stickLimitLeftRight.invert;
        break;
    case INV_UP_DOWN:
        radioData.analogToDigitalData.stickLimitUpDown.invert = !radioData.analogToDigitalData.stickLimitUpDown.invert;
        break;
    case INV_SLIDER:
        radioData.analogToDigitalData.stickLimitSlider.invert = !radioData.analogToDigitalData.stickLimitSlider.invert;
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}
void AnalogToDigital::center()
{
    switch (selectedMenuEntry)
    {
    case LIM_LEFT_RIGHT:
        radioData.analogToDigitalData.stickLimitLeftRight.center = radioData.analogData.stickLeftRight;
        break;
    case LIM_UP_DOWN:
        radioData.analogToDigitalData.stickLimitUpDown.center = radioData.analogData.stickUpDown;
        break;
    case LIM_SLIDER:
        radioData.analogToDigitalData.stickLimitSlider.center = radioData.analogData.slider;
        break;
    case INV_LEFT_RIGHT:
        break;
    case INV_UP_DOWN:
        break;
    case INV_SLIDER:
        break;
    default:
        break;
    }
}

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
    radioData.rawData.menu = analogRead(PIN_MENU);

    radioData.rawData.stickUpDown = 0;
    radioData.rawData.stickLeftRight = 0;
    radioData.rawData.slider = 0;
    radioData.rawData.battery = 0;
    for(int i=0;i<10;i++){
        radioData.rawData.battery += analogRead(PIN_BAT_VOLTAGE);
        radioData.rawData.stickUpDown += analogRead(PIN_JOY_PITCH);
        radioData.rawData.stickLeftRight += analogRead(PIN_JOY_ROLL);
        radioData.rawData.slider += analogRead(PIN_THROTTLE);
    }
    radioData.rawData.battery = radioData.rawData.battery/10;    
    radioData.rawData.stickUpDown = radioData.rawData.stickUpDown/10;
    radioData.rawData.stickLeftRight = radioData.rawData.stickLeftRight/10;
    radioData.rawData.slider = radioData.rawData.slider/10;

    radioData.analogData.battery = radioData.rawData.battery*3.3/4096.0*2;
    radioData.analogData.menu = radioData.rawData.menu*3.1/4096.0; //TODO sollten alle 3.1V -> 3.3V sein? wenn ja muss default umgerechnet werden.
    radioData.analogData.stickUpDown = radioData.rawData.stickUpDown*3.1/4096.0;
    radioData.analogData.stickLeftRight = radioData.rawData.stickLeftRight*3.1/4096.0;
    radioData.analogData.slider = radioData.rawData.slider*3.1/4096.0;

    radioData.digitalData.stickUpDown = analogToDigital(radioData.analogData.stickUpDown, radioData.analogToDigitalData.stickLimitUpDown);
    radioData.digitalData.stickLeftRight = analogToDigital(radioData.analogData.stickLeftRight, radioData.analogToDigitalData.stickLimitLeftRight);
    radioData.digitalData.slider = analogToDigital(radioData.analogData.slider, radioData.analogToDigitalData.stickLimitSlider);

    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.up, radioData.analogToDigitalData.menuButtonTolerance, changeTimeUpMs, radioData.digitalData.up, radioData.digitalData.upEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.down, radioData.analogToDigitalData.menuButtonTolerance, changeTimeDownMs, radioData.digitalData.down, radioData.digitalData.downEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.left, radioData.analogToDigitalData.menuButtonTolerance, changeTimeLeftMs, radioData.digitalData.left, radioData.digitalData.leftEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.right, radioData.analogToDigitalData.menuButtonTolerance, changeTimeRightMs, radioData.digitalData.right, radioData.digitalData.rightEvent);
    getMenuButton(radioData.analogData.menu, radioData.analogToDigitalData.menuButtonLimit.center, radioData.analogToDigitalData.menuButtonTolerance, changeTimeCenterMs, radioData.digitalData.center, radioData.digitalData.centerEvent);
    radioData.digitalData.sideSwitch = digitalRead(PIN_SWITCH);

    getLongPress(radioData.digitalData.upLongPressEvent,startPressTimeUpMs,radioData.digitalData.up,radioData.digitalData.upEvent);
    getLongPress(radioData.digitalData.downLongPressEvent,startPressTimeDownMs,radioData.digitalData.down,radioData.digitalData.downEvent);
    getLongPress(radioData.digitalData.leftLongPressEvent,startPressTimeLeftMs,radioData.digitalData.left,radioData.digitalData.leftEvent);
    getLongPress(radioData.digitalData.rightLongPressEvent,startPressTimeRightMs,radioData.digitalData.right,radioData.digitalData.rightEvent);
    getLongPress(radioData.digitalData.centerLongPressEvent,startPressTimeCenterMs,radioData.digitalData.center,radioData.digitalData.centerEvent);
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