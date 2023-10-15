#ifndef RADIO_CLASS_HPP
#define RADIO_CLASS_HPP

#include <TFT_eSPI.h>
#include <Arduino.h>
#include <RadioData.hpp>
#include <string.h>

class RadioClass
{

protected:
    TFT_eSPI& tft;
    RadioData& radioData;
    char myString[40] = "Menu Text";
    int posH = 320/2;
    int posW = 20;
    int incH = 20;
    void drawMenuPointer(int selectedMenuEntry, int numberOfMenuEntries);
    void limitValue(double& value);
public:
    RadioClass(TFT_eSPI& newTft, RadioData& newRadioData): tft(newTft), radioData(newRadioData){}
    virtual void doFunction();
    virtual void showMenu();
    virtual void showValue();
    virtual const char * getTitle();
    virtual void up();
    virtual void down();
    virtual bool left();
    virtual bool right();
    virtual void center();
};

void RadioClass::limitValue(double& value)
{
    if(value > 1) value = 1;
    if(value < -1) value = -1;
}

void RadioClass::drawMenuPointer(int selectedMenuEntry, int numberOfMenuEntries)
{
    for(int i=0; i<numberOfMenuEntries; i++)
    {
        tft.drawTriangle(0,posH+incH*i,0,posH+incH+incH*i,posW/1.5,posH+incH/2+incH*i,TFT_BLACK);
    }
    tft.drawTriangle(0,50,0,70,posW/1.5,60,TFT_BLACK);

    if(selectedMenuEntry != numberOfMenuEntries) tft.drawTriangle(0,posH+incH*selectedMenuEntry,0,posH+incH+incH*selectedMenuEntry,posW/1.5,posH+incH/2+incH*selectedMenuEntry,TFT_WHITE);
    else tft.drawTriangle(0,50,0,70,posW/1.5,60,TFT_WHITE);
}


#endif