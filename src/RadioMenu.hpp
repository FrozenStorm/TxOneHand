#ifndef RADIO_MENU_HPP
#define RADIO_MENU_HPP

#include "RadioClass.hpp"
#include <list>
#include <iterator>
#include "DigitalValues.hpp"

class RadioMenu
{
private:
    std::list<RadioClass*>             menuPoints;
    std::list<RadioClass*>::iterator   menuPointer;
    DigitalValues& digitalValues;
public:
    RadioMenu(RadioClass* menuEntry, DigitalValues& newDigitalValues);
    void addEntry(RadioClass* menuEntry);
    void showMenu();
    void nextEntry();
    void previousEntry();
    void processInputs();
};

RadioMenu::RadioMenu(RadioClass* menuEntry, DigitalValues& newDigitalValues) : digitalValues(newDigitalValues)
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
    
    (*menuPointer)->showMenu();
}

void RadioMenu::nextEntry()
{
    menuPointer++;
    if(menuPointer == menuPoints.end()) menuPointer = menuPoints.begin();
}

void RadioMenu::previousEntry()
{
    if(menuPointer == menuPoints.begin()) menuPointer = menuPoints.end();
    menuPointer--;
}

void RadioMenu::processInputs()
{
    if(digitalValues.upEvent == true) (*menuPointer)->up();
    if(digitalValues.downEvent == true) (*menuPointer)->down();
    if(digitalValues.leftEvent == true) (*menuPointer)->left();
    if(digitalValues.rightEvent == true) (*menuPointer)->right();
    if(digitalValues.centerEvent == true) nextEntry();
}


#endif