#ifndef ANALOG_INPUTS_HPP
#define ANALOG_INPUTS_HPP

#include "RadioClass.hpp"

#define PIN_JOY_PITCH         1
#define PIN_JOY_ROLL          2
#define PIN_THROTTLE          3
#define PIN_BAT_VOLTAGE       4
#define PIN_MENU              10 

#define PIN_SWITCH            11  

class AnalogInputs : public RadioClass
{
private:
    typedef struct {
        double stickUpDown = 0;
        double stickLeftRight = 0;
        double slider = 0;
        double menu = 0;
        double battery = 0;
    } values;
    values raw, analog;

    typedef struct {
        double min = 0;
        double max = 3.1;
        double center = max/2;
        bool invert = false;
    } limits;
    limits limitsStickUpDown, limitsStickLeftRight, limitsSlider;

    double upLimit, downLimit, leftLimit, rightLimit, centerLimit;
    double menuTolerance = 0.1;

    void getMenuButton(double& value, double& limit, double& tolerance, bool& button, bool& buttonEvent);
    double analogToDigital(double value, limits& limit);
public:
    AnalogInputs(TFT_eSPI& newTft, DigitalValues& newDigitalValues);
    void doFunction();
    void showValue();

    void showMenu();
    void showTitle();
    void up();
    void down();
    void left();
    void right();
    void center();
};

void AnalogInputs::showMenu(){};
void AnalogInputs::showTitle(){};
void AnalogInputs::up(){};
void AnalogInputs::down(){};
void AnalogInputs::left(){};
void AnalogInputs::right(){};
void AnalogInputs::center(){};

AnalogInputs::AnalogInputs(TFT_eSPI& newTft, DigitalValues& newDigitalValues) : RadioClass(newTft, newDigitalValues)
{
    analogSetAttenuation(ADC_11db); // 0 mV ~ 3100 mV
    //analogReadResolution(16); // Effective ADC resolution 13 bits
    adcAttachPin(PIN_JOY_ROLL);
    adcAttachPin(PIN_JOY_PITCH);
    adcAttachPin(PIN_THROTTLE);
    adcAttachPin(PIN_MENU);
    adcAttachPin(PIN_BAT_VOLTAGE);

    pinMode(PIN_SWITCH, INPUT_PULLUP);

    limitsSlider.min = 0.25;
    limitsSlider.max = 1.45;
    limitsSlider.center = 0.85;
    limitsSlider.invert = true;

    limitsStickLeftRight.min = 0.13;
    limitsStickLeftRight.max = 2.93;
    limitsStickLeftRight.center = 1.5;
    limitsStickLeftRight.invert = false;

    limitsStickUpDown.min = 0.01;
    limitsStickUpDown.max = 3.01;
    limitsStickUpDown.center = 1.34;
    limitsStickUpDown.invert = true;
    
    upLimit = 1.67;
    downLimit = 0.53;
    leftLimit = 0.38;
    rightLimit = 1.38;
    centerLimit = 0.96;
}

void AnalogInputs::doFunction()
{
    raw.battery = 0;
    raw.menu = 0;
    raw.slider = 0;
    raw.stickLeftRight = 0;
    raw.stickUpDown = 0;
    for(int i=0;i<10;i++){
        raw.battery += analogRead(PIN_BAT_VOLTAGE);
        raw.menu += analogRead(PIN_MENU);
        raw.slider += analogRead(PIN_THROTTLE);
        raw.stickLeftRight += analogRead(PIN_JOY_ROLL);
        raw.stickUpDown += analogRead(PIN_JOY_PITCH);
    }
    raw.battery = raw.battery/10;
    raw.menu = raw.menu/10;
    raw.slider = raw.slider/10;
    raw.stickLeftRight = raw.stickLeftRight/10;
    raw.stickUpDown = raw.stickUpDown/10;
    

    analog.battery = raw.battery*3.1/4096.0*2;
    analog.menu = raw.menu*3.1/4096.0;
    analog.slider = raw.slider*3.1/4096.0;
    analog.stickLeftRight = raw.stickLeftRight*3.1/4096.0;
    analog.stickUpDown = raw.stickUpDown*3.1/4096.0;

    digitalValues.slider = analogToDigital(analog.slider, limitsSlider);
    digitalValues.stickLeftRight = analogToDigital(analog.stickLeftRight, limitsStickLeftRight);
    digitalValues.stickUpDown = analogToDigital(analog.stickUpDown, limitsStickUpDown);
    digitalValues.batteryVoltage = analog.battery;
    getMenuButton(analog.menu, upLimit, menuTolerance, digitalValues.upValue, digitalValues.upEvent);
    getMenuButton(analog.menu, downLimit, menuTolerance, digitalValues.downValue, digitalValues.downEvent);
    getMenuButton(analog.menu, leftLimit, menuTolerance, digitalValues.leftValue, digitalValues.leftEvent);
    getMenuButton(analog.menu, rightLimit, menuTolerance, digitalValues.rightValue, digitalValues.rightEvent);
    getMenuButton(analog.menu, centerLimit, menuTolerance, digitalValues.centerValue, digitalValues.centerEvent);
    digitalValues.switchPos = digitalRead(PIN_SWITCH);
}

void AnalogInputs::showValue()
{
    tft.print("PIN_BAT_VOLTAGE=");
    tft.println(analog.battery);
    //tft.print("PIN_MENU=");
    //tft.println(analog.menu);
    //tft.print("PIN_THROTTLE=");
    //tft.println(digitalValues.slider);
    //tft.print("PIN_JOY_ROLL=");
    //tft.println(digitalValues.stickLeftRight);
    //tft.print("PIN_JOY_PITCH=");
    //tft.println(digitalValues.stickUpDown);
    tft.print("PIN_SWITCH=");
    tft.println(digitalValues.switchPos);
    tft.print("Value udlrc=");
    tft.print(digitalValues.upValue);
    tft.print(digitalValues.downValue);
    tft.print(digitalValues.leftValue);
    tft.print(digitalValues.rightValue);
    tft.println(digitalValues.centerValue);
}

void AnalogInputs::getMenuButton(double& value, double& limit, double& tolerance, bool& button, bool& buttonEvent)
{
    bool newButtonState;
    if(value <= limit + tolerance && value >= limit - tolerance) newButtonState = true;
    else newButtonState = false;

    if(button == false && newButtonState == true) buttonEvent = true;
    else buttonEvent = false;

    button = newButtonState;
}

double AnalogInputs::analogToDigital(double value, limits& limit)
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