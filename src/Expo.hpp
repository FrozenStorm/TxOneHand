#ifndef EXPO_HPP
#define EXPO_HPP

#include "RadioClass.hpp"
#include <math.h>

class Expo : public RadioClass
{
private:
    enum MenuEntries{PITCH, ROLL, THROTTLE, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;
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
    char myString[40];
    int posH = tft.height()/2+20;
    int posW = 20;
    int incH = 20;
    sprintf(myString,"Pitch = %.0f%%    \n",radioData.expoData.pitch*100);
    tft.drawString(myString, posW, posH+incH*0);
    sprintf(myString,"Roll = %.0f%%    \n",radioData.expoData.roll*100);
    tft.drawString(myString, posW, posH+incH*1);
    sprintf(myString,"Throttle = %.0f%%    \n",radioData.expoData.throttle*100);
    tft.drawString(myString, posW, posH+incH*2);

    for(int i=0; i<NUMBER_OF_MENUENTRIES; i++)
    {
        tft.drawTriangle(0,posH+incH*i,0,posH+incH+incH*i,posW/1.5,posH+incH/2+incH*i,TFT_BLACK);
    }
    if(selectedMenuEntry != NUMBER_OF_MENUENTRIES) tft.drawTriangle(0,posH+incH*selectedMenuEntry,0,posH+incH+incH*selectedMenuEntry,posW/1.5,posH+incH/2+incH*selectedMenuEntry,TFT_WHITE);
}

void Expo::up()
{
    switch (selectedMenuEntry)
    {
    case PITCH:
        selectedMenuEntry = NUMBER_OF_MENUENTRIES;
        break;
    case ROLL:
        selectedMenuEntry = PITCH;
        break;
    case THROTTLE:
        selectedMenuEntry = ROLL;
        break;
    case NUMBER_OF_MENUENTRIES:
        selectedMenuEntry = THROTTLE;
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
        selectedMenuEntry = NUMBER_OF_MENUENTRIES;
        break;
    case NUMBER_OF_MENUENTRIES:
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