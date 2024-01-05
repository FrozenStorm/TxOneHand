#ifndef EXPO_HPP
#define EXPO_HPP

#include "RadioClass.hpp"
#include <math.h>

class Expo : public RadioClass
{
private:
    float stepSize = 0.05;
    enum MenuEntries{PITCH, ROLL, THROTTLE, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;
    void calcExpo(float& value, const float expo);
public:
    Expo(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData){}
    void doFunction();

    void showMenu();
    void showValue();
    const char * getTitle();
    void up();
    void down();
    bool left();
    bool right();
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
    sprintf(myString,"Pitch = %.0f%%    \n",radioData.expoData.pitch*100);
    tft.drawString(myString, posW, posH+incH*0);
    sprintf(myString,"Roll = %.0f%%    \n",radioData.expoData.roll*100);
    tft.drawString(myString, posW, posH+incH*1);
    sprintf(myString,"Throttle = %.0f%%    \n",radioData.expoData.throttle*100);
    tft.drawString(myString, posW, posH+incH*2);

    drawMenuPointer(selectedMenuEntry,NUMBER_OF_MENUENTRIES);
}

void Expo::up()
{
    if(selectedMenuEntry > 0) selectedMenuEntry=(MenuEntries)(selectedMenuEntry-1);
    else selectedMenuEntry = NUMBER_OF_MENUENTRIES;
}
void Expo::down()
{
    if(selectedMenuEntry < NUMBER_OF_MENUENTRIES) selectedMenuEntry=(MenuEntries)(selectedMenuEntry+1);
    else selectedMenuEntry = (MenuEntries)0;
}
bool Expo::left()
{
    switch (selectedMenuEntry)
    {
    case PITCH:
        radioData.expoData.pitch-=stepSize;
        limitValue(radioData.expoData.pitch);
        break;
    case ROLL:
        radioData.expoData.roll-=stepSize;
        limitValue(radioData.expoData.roll);
        break;
    case THROTTLE:
        radioData.expoData.throttle-=stepSize;
        limitValue(radioData.expoData.throttle);
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}

bool Expo::right()
{
    switch (selectedMenuEntry)
    {
    case PITCH:
        radioData.expoData.pitch+=stepSize;
        limitValue(radioData.expoData.pitch);
        break;
    case ROLL:
        radioData.expoData.roll+=stepSize;
        limitValue(radioData.expoData.roll);
        break;
    case THROTTLE:
        radioData.expoData.throttle+=stepSize;
        limitValue(radioData.expoData.throttle);
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}

void Expo::doFunction()
{
    calcExpo(radioData.functionData.throttle, radioData.expoData.throttle);
    calcExpo(radioData.functionData.roll, radioData.expoData.roll);
    calcExpo(radioData.functionData.pitch, radioData.expoData.pitch);
}

void Expo::calcExpo(float& value, const float expo)
{
    value = ((1 - expo) * value + expo * pow(value,3));
    limitValue(value);
}

#endif