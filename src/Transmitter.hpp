#ifndef TRANSMITTER_HPP
#define TRANSMITTER_HPP

#include "RadioClass.hpp"

class Transmitter : public RadioClass
{
private:
    enum MenuEntries{BIND, PROTOCOL, SUBPROTOCOL, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;
public:
    Transmitter(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData){}
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


void Transmitter::showValue(){};
const char * Transmitter::getTitle()
{
    return "Transmitter";
}

void Transmitter::showMenu()
{
    sprintf(myString,"State = %-16s\n",radioData.bindingStateNames[radioData.transmitterData.bindingState]);
    tft.drawString(myString, posW, posH+incH*0);

    sprintf(myString,"Prot. = %-16s\n",radioData.protocolList[radioData.transmitterData.selectedProtocol].name);
    tft.drawString(myString, posW, posH+incH*1);

    sprintf(myString,"Sub P. = %-16s\n",radioData.protocolList[radioData.transmitterData.selectedProtocol].subProtocolList[radioData.transmitterData.selectedSubProtocol].name);
    tft.drawString(myString, posW, posH+incH*2);

    drawMenuPointer(selectedMenuEntry,NUMBER_OF_MENUENTRIES);
}

void Transmitter::up()
{
    if(selectedMenuEntry > 0) selectedMenuEntry=(MenuEntries)(selectedMenuEntry-1);
    else selectedMenuEntry = NUMBER_OF_MENUENTRIES;
}
void Transmitter::down()
{
    if(selectedMenuEntry < NUMBER_OF_MENUENTRIES) selectedMenuEntry=(MenuEntries)(selectedMenuEntry+1);
    else selectedMenuEntry = (MenuEntries)0;
}
bool Transmitter::left()
{
    switch (selectedMenuEntry)
    {
    case BIND:
        break;
    case PROTOCOL:
        radioData.transmitterData.selectedSubProtocol = 0;
        if(radioData.transmitterData.selectedProtocol > 0) radioData.transmitterData.selectedProtocol--;
        else radioData.transmitterData.selectedProtocol = radioData.protocolList.size()-1;
        break;
    case SUBPROTOCOL:
        if(radioData.transmitterData.selectedSubProtocol > 0) radioData.transmitterData.selectedSubProtocol--;
        else radioData.transmitterData.selectedSubProtocol = radioData.protocolList[radioData.transmitterData.selectedProtocol].subProtocolList.size()-1;
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}
bool Transmitter::right()
{
    switch (selectedMenuEntry)
    {
    case BIND:
        break;
    case PROTOCOL:
        radioData.transmitterData.selectedSubProtocol = 0;
        if(radioData.transmitterData.selectedProtocol < radioData.protocolList.size()-1) radioData.transmitterData.selectedProtocol++;
        else radioData.transmitterData.selectedProtocol = 0;
        break;
    case SUBPROTOCOL:
        if(radioData.transmitterData.selectedSubProtocol < radioData.protocolList[radioData.transmitterData.selectedProtocol].subProtocolList.size()-1) radioData.transmitterData.selectedSubProtocol++;
        else radioData.transmitterData.selectedSubProtocol = 0;
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}

void Transmitter::center()
{
    switch (selectedMenuEntry)
    {
    case BIND:
        if(radioData.transmitterData.bindingState == radioData.BINDED || radioData.transmitterData.bindingState == radioData.BINDING_FAILED){
            radioData.transmitterData.bindingState = radioData.BINDING_STARTED;
        }
        break;
    case PROTOCOL:
        break;
    case SUBPROTOCOL:
        break;
    case NUMBER_OF_MENUENTRIES:
        break;
    default:
        break;
    }
}

void Transmitter::doFunction()
{
    static uint32_t bindTimeout = 0;
    const char search[3] = {0x4D,0x50,0x01};
    static bool rxBindFlag = false;

    // Calculate Channel Data
    for(int i = 0; i < 16*11; i++){
        if(radioData.channelData.channel[i/11] & (0x01 << (i % 11))){
            radioData.transmitterData.txData[4+i/8] |= (0x01 << (i % 8));
        }
        else{
            radioData.transmitterData.txData[4+i/8] &= ~(0x01 << (i % 8));
        }
    }

    // Set Protocol
    radioData.transmitterData.txData[1] &= 0xE0;
    radioData.transmitterData.txData[1] |= radioData.protocolList[radioData.transmitterData.selectedProtocol].value & 0x1F;

    // Set Sub Protocol
    radioData.transmitterData.txData[2] &= 0x8F;
    radioData.transmitterData.txData[2] |= (radioData.protocolList[radioData.transmitterData.selectedProtocol].subProtocolList[radioData.transmitterData.selectedSubProtocol].value << 4) & 0x70;    

    // Read binding progress
    if(radioData.transmitterData.bindingState == radioData.BINDED || radioData.transmitterData.bindingState == radioData.BINDING_FAILED){}
    else{
        if(Serial2.find(search,3))
        {
            if(0!=Serial2.readBytes(radioData.transmitterData.rxData,10))
            {
                if((radioData.transmitterData.rxData[1] & 0x08) == 0x08) rxBindFlag = true;
                else rxBindFlag = false;
            }
        }
    }

    // Set binding bit
    switch (radioData.transmitterData.bindingState)
    {
        case radioData.BINDED:
        case radioData.BINDING_FAILED:
            bindTimeout = millis() + 15000;
            rxBindFlag = false;
            Serial2.flush();
            break;
        case radioData.BINDING_STARTED:
            radioData.transmitterData.txData[1] |= 0x80;
            if(rxBindFlag == true) radioData.transmitterData.bindingState = radioData.BINDING;
            if(bindTimeout < millis()) radioData.transmitterData.bindingState = radioData.BINDING_FAILED;
            break;
        case radioData.BINDING:
            radioData.transmitterData.txData[1] |= 0x80;
            if(rxBindFlag == false) radioData.transmitterData.bindingState = radioData.BINDING_FINISHED;
            if(bindTimeout < millis()) radioData.transmitterData.bindingState = radioData.BINDING_FAILED;
            break;
        case radioData.BINDING_FINISHED:
            radioData.transmitterData.txData[1] &= ~0x80;
            if(bindTimeout < millis()) radioData.transmitterData.bindingState = radioData.BINDED;
            break;  
        default:
            break;
    }

    Serial1.write(radioData.transmitterData.txData,27);
}

#endif