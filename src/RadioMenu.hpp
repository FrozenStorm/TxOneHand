#ifndef RADIO_MENU_HPP
#define RADIO_MENU_HPP

#include "RadioClass.hpp"
#include <list>
#include <iterator>
#include "RadioData.hpp"

class RadioMenu
{
private:
    TFT_eSPI&                          tft;
    std::list<RadioClass*>             menuPoints;
    std::list<RadioClass*>::iterator   menuPointer;
    RadioData& radioData;
public:
    RadioMenu(TFT_eSPI& newTft, RadioData& newDigitalValues, RadioClass* menuEntry);
    void addEntry(RadioClass* menuEntry);
    void showMenu();
    void nextEntry();
    void previousEntry();
    void processInputs();
};

RadioMenu::RadioMenu(TFT_eSPI& newTft, RadioData& newRadioData, RadioClass* menuEntry) : tft(newTft), radioData(newRadioData)
{
        menuPoints.push_back(menuEntry);
        menuPointer = menuPoints.begin();    
}

void RadioMenu::addEntry(RadioClass* menuEntry)
{
    menuPoints.push_back(menuEntry);
}

void RadioMenu::showMenu()
{
    char myString[10];
    tft.fillScreen(TFT_BLACK);

    tft.setTextDatum(MC_DATUM);
    previousEntry();
    tft.setTextSize(1);
    tft.drawString((*menuPointer)->getTitle(),tft.width()/2, 30,2);
    nextEntry();
    tft.setTextSize(2);
    tft.drawString((*menuPointer)->getTitle(),tft.width()/2, 60,2);
    nextEntry();
    tft.setTextSize(1);
    tft.drawString((*menuPointer)->getTitle(),tft.width()/2, 90,2);
    previousEntry();

    tft.setTextSize(1);
    tft.drawString("TxOneHand by Z-Craft",tft.width()/2, tft.height()/2,2);

    tft.setTextDatum(TL_DATUM);
    sprintf(myString,"%1.1fV",radioData.analogData.battery);
    tft.drawString(myString,0,0,2);

    tft.setCursor(0, tft.height()/2, 2);
    tft.println();


    (*menuPointer)->showMenu();
}

void RadioMenu::nextEntry()
{
    menuPointer++;
    radioData.storeData();
    if(menuPointer == menuPoints.end()) menuPointer = menuPoints.begin();
}

void RadioMenu::previousEntry()
{
    if(menuPointer == menuPoints.begin()) menuPointer = menuPoints.end();
    menuPointer--;
}

void RadioMenu::processInputs()
{
    if(radioData.digitalData.upEvent == true) (*menuPointer)->up();
    if(radioData.digitalData.downEvent == true) (*menuPointer)->down();
    if(radioData.digitalData.leftEvent == true) (*menuPointer)->left();
    if(radioData.digitalData.rightEvent == true) (*menuPointer)->right();
    if(radioData.digitalData.centerLongPressEvent == true) nextEntry();
}


#endif