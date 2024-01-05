#ifndef MIXER_HPP
#define MIXER_HPP

#include "RadioClass.hpp"

class Mixer : public RadioClass
{
private:
    float stepSize = 0.05;
    enum MenuEntries{THROTTLE, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;

public:
    Mixer(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData){}
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


void Mixer::showValue(){};
const char * Mixer::getTitle()
{
    return "Mixer";
}
void Mixer::center(){};

void Mixer::showMenu()
{
    sprintf(myString,"T -> P = %.0f%%    \n",radioData.mixerData.throttleToPitch*100);
    tft.drawString(myString, posW, posH+incH*0);

    drawMenuPointer(selectedMenuEntry,NUMBER_OF_MENUENTRIES);
}

void Mixer::up()
{
    if(selectedMenuEntry > 0) selectedMenuEntry=(MenuEntries)(selectedMenuEntry-1);
    else selectedMenuEntry = NUMBER_OF_MENUENTRIES;
}
void Mixer::down()
{
    if(selectedMenuEntry < NUMBER_OF_MENUENTRIES) selectedMenuEntry=(MenuEntries)(selectedMenuEntry+1);
    else selectedMenuEntry = (MenuEntries)0;
}
bool Mixer::left()
{
    switch (selectedMenuEntry)
    {
    case THROTTLE:
        radioData.mixerData.throttleToPitch-=stepSize;
        limitValue(radioData.mixerData.throttleToPitch);
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}
bool Mixer::right()
{
    switch (selectedMenuEntry)
    {
    case THROTTLE:
        radioData.mixerData.throttleToPitch+=stepSize;
        limitValue(radioData.mixerData.throttleToPitch);
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}

void Mixer::doFunction()
{
    radioData.functionData.pitch += radioData.mixerData.throttleToPitch * ((radioData.functionData.throttle + 1) / 2);
}



#endif