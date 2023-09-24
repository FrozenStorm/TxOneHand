#ifndef EXPO_HPP
#define EXPO_HPP

#include "RadioClass.hpp"
#include <math.h>

class Expo : public RadioClass
{
private:
    double expoStickLeftRight = 0.3;
    double expoStickUpDown = 0.3;
    double expoSlider = 0;
    void calcExpo(double& value, double expo);
public:
    Expo(TFT_eSPI& newTft, DigitalValues& newDigitalValues) : RadioClass(newTft, newDigitalValues){}
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

void Expo::showValue(){};
void Expo::showTitle(){};
void Expo::center(){};

void Expo::showMenu()
{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(15, 140, 2);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);  
    tft.setTextSize(1);

    tft.println("TxOneHand by Z-Craft");
    tft.println();

    tft.println("Expo");

    tft.printf("expoStickLeftRight = %.0f%%\n",expoStickLeftRight*100);

    tft.printf("expoStickUpDown = %.0f%%\n",expoStickUpDown*100);
    
    tft.printf("expoSlider = %.0f%%\n",expoSlider*100);
}

void Expo::up()
{
    expoStickUpDown+=0.05;
    limitValue(expoStickUpDown);
}
void Expo::down()
{
    expoStickUpDown-=0.05;
    limitValue(expoStickUpDown);
}
void Expo::left()
{
    expoStickLeftRight-=0.05;
    limitValue(expoStickLeftRight);
}
void Expo::right()
{
    expoStickLeftRight+=0.05;
    limitValue(expoStickLeftRight);
}

void Expo::doFunction()
{
    calcExpo(digitalValues.slider, expoSlider);
    calcExpo(digitalValues.stickLeftRight, expoStickLeftRight);
    calcExpo(digitalValues.stickUpDown, expoStickUpDown);
}

void Expo::calcExpo(double& value, double expo)
{
    value = ((1 - expo) * value + expo * pow(value,3));
    limitValue(value);
}

#endif