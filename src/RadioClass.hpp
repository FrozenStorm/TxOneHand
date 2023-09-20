#ifndef RADIO_CLASS_HPP
#define RADIO_CLASS_HPP

#include <TFT_eSPI.h>
#include <Arduino.h>
#include "DigitalValues.hpp"

class RadioClass
{

protected:
    void limitValue(double& value);
    TFT_eSPI& tft;
    DigitalValues& digitalValues;
    /* data */
public:
    RadioClass(TFT_eSPI& newTft, DigitalValues& newDigitalValues): tft(newTft), digitalValues(newDigitalValues){}
    void doFunction();
    void showMenu();
    void showValue();
    void showTitle();
    void up();
    void down();
    void left();
    void right();
    void center();
};

void RadioClass::limitValue(double& value)
{
    if(value > 1) value = 1;
    if(value < -1) value = -1;
}


#endif