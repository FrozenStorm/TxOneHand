#ifndef TRANSMITTER_HPP
#define TRANSMITTER_HPP

#include "RadioClass.hpp"

#define PIN_MULTI_TX          43
#define PIN_MULTI_RX          44

class Transmitter : public RadioClass
{
private:
    enum MenuEntries{BIND, PROTOCOL, SUBPROTOCOL, RX_NUMBER, POWER_LEVEL, RANGE_CHECK, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;

    uint32_t bindTimeout = 0;
    bool rxBindFlag = false;
    unsigned char txData[27] = {0x55,0x06,0x20,0x07,0x00,0x24,0x20,0x07,0x01,0x08,0x40,0x00,0x02,0x10,0x80,0x00,0x04,0x20,0x00,0x01,0x08,0x40,0x00,0x02,0x10,0x80,0x08}; // TODO wiso muss hier unsigend char stehen, damit das init mit 0xE3 funktioniert
    unsigned char rxData[28] = {0x4D,0x50,0x01,0x18,0x47,0x01,0x03,0x03,0x14,0xE4,0x46,0x21,0x44,0x53,0x4D,0x00,0x4F,0x4D,0x50,0x76,0x58,0x20,0x31,0x46,0x00,0x00,0x00,0x00};
public:
    Transmitter(TFT_eSPI& newTft, RadioData& newRadioData);
    void doFunction();
    bool sendTx(void *);

    void showMenu();
    void showValue();
    const char * getTitle();
    void up();
    void down();
    bool left();
    bool right();
    void center();
};

Transmitter::Transmitter(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData)
{
  // UART
  Serial1.begin(100000, SERIAL_8E2, -1, PIN_MULTI_TX);
  Serial2.begin(100000, SERIAL_8E2, PIN_MULTI_RX, -1, true);
  Serial2.setTimeout(4);
}


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

    sprintf(myString,"RxNum = %-2d\n",radioData.transmitterData.rxNum);
    tft.drawString(myString, posW, posH+incH*3);

    sprintf(myString,"Power = %-8s\n",radioData.powerValueNames[radioData.transmitterData.powerValue]);
    tft.drawString(myString, posW, posH+incH*4);

    sprintf(myString,"Range Check = %d\n",radioData.transmitterData.rangeCheck);
    tft.drawString(myString, posW, posH+incH*5);

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
    case RX_NUMBER:
        if(radioData.transmitterData.rxNum > 0) radioData.transmitterData.rxNum--;
        else radioData.transmitterData.rxNum = 15;
        break;
    case POWER_LEVEL:
        if(radioData.transmitterData.powerValue == radioData.POWER_VALUE_HIGH) radioData.transmitterData.powerValue = radioData.POWER_VALUE_LOW;
        else radioData.transmitterData.powerValue = radioData.POWER_VALUE_HIGH;
        break;
    case RANGE_CHECK:
        radioData.transmitterData.rangeCheck = !radioData.transmitterData.rangeCheck;
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
    case RX_NUMBER:
        if(radioData.transmitterData.rxNum < 15) radioData.transmitterData.rxNum++;
        else radioData.transmitterData.rxNum = 0;
        break;
    case POWER_LEVEL:
        if(radioData.transmitterData.powerValue == radioData.POWER_VALUE_HIGH) radioData.transmitterData.powerValue = radioData.POWER_VALUE_LOW;
        else radioData.transmitterData.powerValue = radioData.POWER_VALUE_HIGH;
        break;
    case RANGE_CHECK:
        radioData.transmitterData.rangeCheck = !radioData.transmitterData.rangeCheck;
        break; 
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false; //TODO alle return umbiegen, dass error und ok richtig sind
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
    const char search[3] = {0x4D,0x50,0x01};

    // Read binding progress
    if(radioData.transmitterData.bindingState == radioData.BINDED || radioData.transmitterData.bindingState == radioData.BINDING_FAILED){}
    else{
        if(Serial2.find(search,3))
        {
            if(0!=Serial2.readBytes(rxData,10))
            {
                if((rxData[1] & 0x08) == 0x08) rxBindFlag = true;
                else rxBindFlag = false;
            }
        }
    }

    // Set binding state
    switch (radioData.transmitterData.bindingState)
    {
        case radioData.BINDED:
        case radioData.BINDING_FAILED:
            bindTimeout = millis() + 15000;
            rxBindFlag = false;
            Serial2.flush();
            break;
        case radioData.BINDING_STARTED:
            if(rxBindFlag == true) radioData.transmitterData.bindingState = radioData.BINDING;
            if(bindTimeout < millis()) radioData.transmitterData.bindingState = radioData.BINDING_FAILED;
            break;
        case radioData.BINDING:
            if(rxBindFlag == false) radioData.transmitterData.bindingState = radioData.BINDING_FINISHED;
            if(bindTimeout < millis()) radioData.transmitterData.bindingState = radioData.BINDING_FAILED;
            break;
        case radioData.BINDING_FINISHED:
            if(bindTimeout < millis()) radioData.transmitterData.bindingState = radioData.BINDED;
            break;  
        default:
            break;
    }

    sendTx(NULL);
}

bool Transmitter::sendTx(void *)
{
    // Calculate Channel Data
    for(int i = 0; i < 16*11; i++){
        if(radioData.channelData.channel[i/11] & (0x01 << (i % 11))){
            txData[4+i/8] |= (0x01 << (i % 8));
        }
        else{
            txData[4+i/8] &= ~(0x01 << (i % 8));
        }
    }

    // Set Protocol
    txData[1] &= 0xD0;
    txData[1] |= (radioData.transmitterData.rangeCheck << 5) & 0x20;

    // Set Range Check
    txData[1] &= 0xE0;
    txData[1] |= radioData.protocolList[radioData.transmitterData.selectedProtocol].value & 0x1F;

    // Set Sub Protocol
    txData[2] &= 0x8F;
    txData[2] |= (radioData.protocolList[radioData.transmitterData.selectedProtocol].subProtocolList[radioData.transmitterData.selectedSubProtocol].value << 4) & 0x70;    

    // Set RxNum
    txData[2] &= 0xF0;
    txData[2] |= radioData.transmitterData.rxNum & 0x0F;    

    // Set Power
    txData[2] &= 0x70;
    txData[2] |= (radioData.transmitterData.powerValue) << 7 & 0x80;   

    // Set binding bit
    switch (radioData.transmitterData.bindingState)
    {
        case radioData.BINDED:
        case radioData.BINDING_FAILED:
            break;
        case radioData.BINDING_STARTED:
            txData[1] |= 0x80;
            break;
        case radioData.BINDING:
            txData[1] |= 0x80;
            break;
        case radioData.BINDING_FINISHED:
            txData[1] &= ~0x80;
            break;  
        default:
            break;
    }

    // Send Data
    Serial1.write(txData,27);
    return true;
}

#endif