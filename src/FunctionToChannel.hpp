#pragma once

#include "RadioClass.hpp"

class FunctionToChannel : public RadioClass
{
private:
public:
    FunctionToChannel(RadioData& newRadioData) : RadioClass(newRadioData){}
    void doFunction() override;
};

void FunctionToChannel::doFunction()
{  
    for(int i = 0; i < SUPPORTED_CHANNELS; i++){
        float value;
        switch(radioData.functionToChannelData.functionOnChannel[i])
        {
            case radioData.NONE:
                value = 0;
                break;
            case radioData.PITCH:
                value = radioData.functionData.pitch;
                break;
            case radioData.ROLL:
                value = radioData.functionData.roll;
                break;
            case radioData.VTAIL_LEFT:
                value = radioData.functionData.vTailLeft;
                break;
            case radioData.VTAIL_RIGHT:
                value = radioData.functionData.vTailRight;
                break;
            case radioData.THROTTLE:
                value = radioData.functionData.throttle;
                break;            
            case radioData.STICK_LR:
                value = radioData.digitalData.stickLeftRight;
                break;
            case radioData.STICK_UD:
                value = radioData.digitalData.stickUpDown;
                break;
            default:
                value = 0;
                break;
        }
        if(radioData.functionToChannelData.invertChannel[i] == true) value = -value;

        value = (value + 1) * 1023;

        if(value > radioData.functionToChannelData.upperLimitChannel[i]) value = radioData.functionToChannelData.upperLimitChannel[i];
        if(value < radioData.functionToChannelData.lowerLimitChannel[i]) value = radioData.functionToChannelData.lowerLimitChannel[i];

        radioData.channelData.channel[i] = value; // Nur eine Zuweisung damit es Interrupt sicher ist
    }
    radioData.channelData.channel[13] = radioData.channelData.channel[2]; // Failsafe oder so Begrenzung auf Kanal 14 von Throttle
}