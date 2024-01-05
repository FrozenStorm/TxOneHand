#ifndef DUAL_RATE_HPP
#define DUAL_RATE_HPP

#include "RadioClass.hpp"

class DualRate : public RadioClass
{
private:
    float stepSize = 0.05;
    enum MenuEntries{PITCH, ROLL, THROTTLE, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;
    void calcRate(float& value, float rate);
public:
    DualRate(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData){}
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


void DualRate::showValue(){};
const char * DualRate::getTitle()
{
    return "DualRate";
}
void DualRate::center(){};

void DualRate::showMenu()
{
    sprintf(myString,"Pitch = %.0f%%    \n",radioData.dualRateData.pitch*100);
    tft.drawString(myString, posW, posH+incH*0);
    sprintf(myString,"Roll = %.0f%%    \n",radioData.dualRateData.roll*100);
    tft.drawString(myString, posW, posH+incH*1);
    sprintf(myString,"Throttle = %.0f%%    \n",radioData.dualRateData.throttle*100);
    tft.drawString(myString, posW, posH+incH*2);

    drawMenuPointer(selectedMenuEntry,NUMBER_OF_MENUENTRIES);
}

void DualRate::up()
{
    if(selectedMenuEntry > 0) selectedMenuEntry=(MenuEntries)(selectedMenuEntry-1);
    else selectedMenuEntry = NUMBER_OF_MENUENTRIES;
}
void DualRate::down()
{
    if(selectedMenuEntry < NUMBER_OF_MENUENTRIES) selectedMenuEntry=(MenuEntries)(selectedMenuEntry+1);
    else selectedMenuEntry = (MenuEntries)0;
}
bool DualRate::left()
{
    switch (selectedMenuEntry)
    {
    case PITCH:
        radioData.dualRateData.pitch-=stepSize;
        if(radioData.dualRateData.pitch < 0 ) radioData.dualRateData.pitch = 0;
        break;
    case ROLL:
        radioData.dualRateData.roll-=stepSize;
        if(radioData.dualRateData.roll < 0 ) radioData.dualRateData.roll = 0;
        break;
    case THROTTLE:
        radioData.dualRateData.throttle-=stepSize;
        if(radioData.dualRateData.throttle < 0 ) radioData.dualRateData.throttle = 0;
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}
bool DualRate::right()
{
        switch (selectedMenuEntry)
    {
    case PITCH:
        radioData.dualRateData.pitch+=stepSize;
        limitValue(radioData.dualRateData.roll);
        break;
    case ROLL:
        radioData.dualRateData.roll+=stepSize;
        limitValue(radioData.dualRateData.roll);
        break;
    case THROTTLE:
        radioData.dualRateData.throttle+=stepSize;
        limitValue(radioData.dualRateData.roll);
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}

void DualRate::doFunction()
{
    calcRate(radioData.functionData.throttle, radioData.dualRateData.throttle);
    calcRate(radioData.functionData.roll, radioData.dualRateData.roll);
    calcRate(radioData.functionData.pitch, radioData.dualRateData.pitch);
}

void DualRate::calcRate(float& value, float rate)
{
    if(value > rate) value = rate;
    if(value < -rate) value = -rate;
}


#endif