#ifndef FUNCTION_TO_CHANNEL_HPP
#define FUNCTION_TO_CHANNEL_HPP

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
    float vtail_left = (-radioData.functionData.roll - radioData.functionData.pitch);
    limitValue(vtail_left);
    float vtail_right = (-radioData.functionData.roll + radioData.functionData.pitch);
    limitValue(vtail_right);  
  
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
                value = vtail_left;
                break;
            case radioData.VTAIL_RIGHT:
                value = vtail_right;
                break;
            case radioData.THROTTLE:
                value = radioData.functionData.throttle;
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

#endif