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
};

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