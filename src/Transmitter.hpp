#pragma once

#include "RadioClass.hpp"
#include "driver/uart.h"

#define PIN_MULTI_TX          14
#define PIN_MULTI_RX          13

class Transmitter : public RadioClass
{
private:
    uint32_t bindTimeout = 0;
    bool rxBindFlag = false;
    unsigned char txData[27] = {0x55,0x06,0x20,0x07,0x00,0x24,0x20,0x07,0x01,0x08,0x40,0x00,0x02,0x10,0x80,0x00,0x04,0x20,0x00,0x01,0x08,0x40,0x00,0x02,0x10,0x80,0x08}; // TODO wiso muss hier unsigend char stehen, damit das init mit 0xE3 funktioniert
    unsigned char rxData[28] = {0x4D,0x50,0x01,0x18,0x47,0x01,0x03,0x03,0x14,0xE4,0x46,0x21,0x44,0x53,0x4D,0x00,0x4F,0x4D,0x50,0x76,0x58,0x20,0x31,0x46,0x00,0x00,0x00,0x00};
public:
    Transmitter(RadioData& newRadioData);
    void doFunction();
    bool sendTx(void *);
};

Transmitter::Transmitter(RadioData& newRadioData) : RadioClass(newRadioData)
{
    // UART
    Serial1.begin(100000, SERIAL_8E2, -1, PIN_MULTI_TX);
    Serial2.begin(100000, SERIAL_8E2, PIN_MULTI_RX, -1, true);
    Serial2.setTimeout(4);
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
    Serial1.write(txData,sizeof(txData)); // TODO HIER LIEGT EIN PROBLEM
    return true;
}
