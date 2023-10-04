#ifndef DUAL_RATE_HPP
#define DUAL_RATE_HPP

#include "RadioClass.hpp"

class DualRate : public RadioClass
{
private:
    enum MenuEntries{PITCH, ROLL, THROTTLE, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;
    void calcRate(double& value, double rate);
public:
    DualRate(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData){}
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


void DualRate::showValue(){};
const char * DualRate::getTitle()
{
    return "DualRate";
}
void DualRate::center(){};

void DualRate::showMenu()
{
    char myString[40];
    int posH = tft.height()/2+20;
    int posW = 20;
    int incH = 20;
    sprintf(myString,"Pitch = %.0f%%    \n",radioData.dualRateData.pitch*100);
    tft.drawString(myString, posW, posH+incH*0);
    sprintf(myString,"Roll = %.0f%%    \n",radioData.dualRateData.roll*100);
    tft.drawString(myString, posW, posH+incH*1);
    sprintf(myString,"Throttle = %.0f%%    \n",radioData.dualRateData.throttle*100);
    tft.drawString(myString, posW, posH+incH*2);

    for(int i=0; i<NUMBER_OF_MENUENTRIES; i++)
    {
        tft.drawTriangle(0,posH+incH*i,0,posH+incH+incH*i,posW/1.5,posH+incH/2+incH*i,TFT_BLACK);
    }
    if(selectedMenuEntry != NUMBER_OF_MENUENTRIES) tft.drawTriangle(0,posH+incH*selectedMenuEntry,0,posH+incH+incH*selectedMenuEntry,posW/1.5,posH+incH/2+incH*selectedMenuEntry,TFT_WHITE);
}

void DualRate::up()
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
void DualRate::down()
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
void DualRate::left()
{
    switch (selectedMenuEntry)
    {
    case PITCH:
        radioData.dualRateData.pitch-=radioData.dualRateData.stepSize;
        if(radioData.dualRateData.pitch < 0 ) radioData.dualRateData.pitch = 0;
        break;
    case ROLL:
        radioData.dualRateData.roll-=radioData.dualRateData.stepSize;
        if(radioData.dualRateData.roll < 0 ) radioData.dualRateData.roll = 0;
        break;
    case THROTTLE:
        radioData.dualRateData.throttle-=radioData.dualRateData.stepSize;
        if(radioData.dualRateData.throttle < 0 ) radioData.dualRateData.throttle = 0;
        break;
    default:
        break;
    }
}
void DualRate::right()
{
        switch (selectedMenuEntry)
    {
    case PITCH:
        radioData.dualRateData.pitch+=radioData.dualRateData.stepSize;
        limitValue(radioData.dualRateData.roll);
        break;
    case ROLL:
        radioData.dualRateData.roll+=radioData.dualRateData.stepSize;
        limitValue(radioData.dualRateData.roll);
        break;
    case THROTTLE:
        radioData.dualRateData.throttle+=radioData.dualRateData.stepSize;
        limitValue(radioData.dualRateData.roll);
        break;
    default:
        break;
    }  
}

void DualRate::doFunction()
{
    calcRate(radioData.functionData.throttle, radioData.dualRateData.throttle);
    calcRate(radioData.functionData.roll, radioData.dualRateData.roll);
    calcRate(radioData.functionData.pitch, radioData.dualRateData.pitch);
}

void DualRate::calcRate(double& value, double rate)
{
    if(value > rate) value = rate;
    if(value < -rate) value = -rate;
}


#endif