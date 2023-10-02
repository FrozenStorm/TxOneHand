#ifndef FUNCTION_TO_CHANNEL_HPP
#define FUNCTION_TO_CHANNEL_HPP

#include "RadioClass.hpp"


class FunctionToChannel : public RadioClass
{
private:
public:
    FunctionToChannel(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData){}
    void doFunction() override;
    void showMenu() override;
    void up() override;
    void down() override;
    void left() override;
    void right() override;
    void center() override;

    void showValue();
    const char * getTitle();
};

void FunctionToChannel::showValue(){};
const char * FunctionToChannel::getTitle()
{
    return "FunctionToChannel";
}

void FunctionToChannel::doFunction()
{
    double vtail_left = (-radioData.functionData.roll - radioData.functionData.pitch);
    limitValue(vtail_left);
    double vtail_right = (-radioData.functionData.roll + radioData.functionData.pitch);
    limitValue(vtail_right);  
  
    for(int i = 0; i < sizeof(radioData.channelData.channel)/sizeof(int); i++){
        double value;
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

        radioData.channelData.channel[i] = (value + 1) * 1023;

        if(radioData.channelData.channel[i] > radioData.functionToChannelData.upperLimitChannel[i]) radioData.channelData.channel[i] = radioData.functionToChannelData.upperLimitChannel[i];
        if(radioData.channelData.channel[i] < radioData.functionToChannelData.lowerLimitChannel[i]) radioData.channelData.channel[i] = radioData.functionToChannelData.lowerLimitChannel[i];
    }
}


void FunctionToChannel::up() 
{

}

void FunctionToChannel::down()
{

}

void FunctionToChannel::left()
{

}

void FunctionToChannel::right()
{

}

void FunctionToChannel::center()
{

}


void FunctionToChannel::showMenu()
{

}


#endif