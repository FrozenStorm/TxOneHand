#ifndef RADIO_CLASS_HPP
#define RADIO_CLASS_HPP

#include <TFT_eSPI.h>
#include <Arduino.h>
#include "DigitalValues.hpp"

class RadioClass
{

protected:
    TFT_eSPI& tft;
    DigitalValues& digitalValues;
    void limitValue(double& value);
public:
    RadioClass(TFT_eSPI& newTft, DigitalValues& newDigitalValues): tft(newTft), digitalValues(newDigitalValues){}
    virtual void doFunction();
    virtual void showMenu();
    virtual void showValue();
    virtual void showTitle();
    virtual void up();
    virtual void down();
    virtual void left();
    virtual void right();
    virtual void center();
};

void RadioClass::limitValue(double& value)
{
    if(value > 1) value = 1;
    if(value < -1) value = -1;
}


#endif