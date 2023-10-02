#ifndef EXPO_HPP
#define EXPO_HPP

#include "RadioClass.hpp"
#include <math.h>

class Expo : public RadioClass
{
private:
    enum MenuEntries{PITCH, ROLL, THROTTLE};
    MenuEntries selectedMenuEntry = PITCH;
    void calcExpo(double& value, const double expo);
public:
    Expo(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData){}
    void doFunction();

    void showMenu();
    void showValue();
    const char * getTitle();
    void up();
    void down();
    void left();
    void right();
    void center();
};

void Expo::showValue(){};
const char * Expo::getTitle()
{
    return "Expo";
}
void Expo::center(){};

void Expo::showMenu()
{
    char myString[20];
    int posH = tft.height()/2+20;
    int posW = 20;
    int incH = 20;
    sprintf(myString,"pitch = %.0f%%\n",radioData.expoData.pitch*100);
    tft.drawString(myString, posW, posH);
    sprintf(myString,"roll = %.0f%%\n",radioData.expoData.roll*100);
    tft.drawString(myString, posW, posH+incH);
    sprintf(myString,"throttle = %.0f%%\n",radioData.expoData.throttle*100);
    tft.drawString(myString, posW, posH+incH*2);

    tft.drawTriangle(0,posH+incH*selectedMenuEntry,0,posH+incH+incH*selectedMenuEntry,posW/1.5,posH+incH/2+incH*selectedMenuEntry,TFT_WHITE);
}

void Expo::up()
{
    switch (selectedMenuEntry)
    {
    case PITCH:
        selectedMenuEntry = THROTTLE;
        break;
    case ROLL:
        selectedMenuEntry = PITCH;
        break;
    case THROTTLE:
        selectedMenuEntry = ROLL;
        break;
    default:
        break;
    }    
}
void Expo::down()
{
    switch (selectedMenuEntry)
    {
    case PITCH:
        selectedMenuEntry = ROLL;
        break;
    case ROLL:
        selectedMenuEntry = THROTTLE;
        break;
    case THROTTLE:
        selectedMenuEntry = PITCH;
        break;
    default:
        break;
    }
}
void Expo::left()
{
    switch (selectedMenuEntry)
    {
    case PITCH:
        radioData.expoData.pitch-=radioData.expoData.stepSize;
        limitValue(radioData.expoData.pitch);
        break;
    case ROLL:
        radioData.expoData.roll-=radioData.expoData.stepSize;
        limitValue(radioData.expoData.roll);
        break;
    case THROTTLE:
        radioData.expoData.throttle-=radioData.expoData.stepSize;
        limitValue(radioData.expoData.throttle);
        break;
    default:
        break;
    }
}
void Expo::right()
{
    switch (selectedMenuEntry)
    {
    case PITCH:
        radioData.expoData.pitch+=radioData.expoData.stepSize;
        limitValue(radioData.expoData.pitch);
        break;
    case ROLL:
        radioData.expoData.roll+=radioData.expoData.stepSize;
        limitValue(radioData.expoData.roll);
        break;
    case THROTTLE:
        radioData.expoData.throttle+=radioData.expoData.stepSize;
        limitValue(radioData.expoData.throttle);
        break;
    default:
        break;
    }
}

void Expo::doFunction()
{
    calcExpo(radioData.functionData.throttle, radioData.expoData.throttle);
    calcExpo(radioData.functionData.roll, radioData.expoData.roll);
    calcExpo(radioData.functionData.pitch, radioData.expoData.pitch);
}

void Expo::calcExpo(double& value, const double expo)
{
    value = ((1 - expo) * value + expo * pow(value,3));
    limitValue(value);
}

#endif