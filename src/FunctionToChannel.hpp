#ifndef FUNCTION_TO_CHANNEL_HPP
#define FUNCTION_TO_CHANNEL_HPP

#include "RadioClass.hpp"

class FunctionToChannel : public RadioClass
{
private:
    int stepSize = 20;
    enum MenuEntries{CHANNEL, FUNCTION, INVERT, LOWER, UPPER, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;
    int selectedMenuChannel = 0;
public:
    FunctionToChannel(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData){}
    void doFunction() override;
    void showMenu() override;
    void up() override;
    void down() override;
    bool left() override;
    bool right() override;
    void center() override;

    void showValue();
    const char * getTitle();
};

void FunctionToChannel::showValue(){};
const char * FunctionToChannel::getTitle()
{
    return "F-TO-C";
}

void FunctionToChannel::showMenu()
{
    sprintf(myString,"Channel = %-2d       \n",selectedMenuChannel+1);
    tft.drawString(myString, posW, posH+incH*0);
    sprintf(myString,"Func. = % -12s       \n",radioData.functionNames[radioData.functionToChannelData.functionOnChannel[selectedMenuChannel]]);
    tft.drawString(myString, posW, posH+incH*1);
    sprintf(myString,"Invert = %d         \n",radioData.functionToChannelData.invertChannel[selectedMenuChannel]);
    tft.drawString(myString, posW, posH+incH*2);
    sprintf(myString,"Min = %-4d       \n",radioData.functionToChannelData.lowerLimitChannel[selectedMenuChannel]);
    tft.drawString(myString, posW, posH+incH*3);
    sprintf(myString,"Max = %-4d        \n",radioData.functionToChannelData.upperLimitChannel[selectedMenuChannel]);
    tft.drawString(myString, posW, posH+incH*4);

    drawMenuPointer(selectedMenuEntry,NUMBER_OF_MENUENTRIES);
}

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


void FunctionToChannel::up()
{
    if(selectedMenuEntry > 0) selectedMenuEntry=(MenuEntries)(selectedMenuEntry-1);
    else selectedMenuEntry = NUMBER_OF_MENUENTRIES;
}
void FunctionToChannel::down()
{
    if(selectedMenuEntry < NUMBER_OF_MENUENTRIES) selectedMenuEntry=(MenuEntries)(selectedMenuEntry+1);
    else selectedMenuEntry = (MenuEntries)0;
}

bool FunctionToChannel::left()
{
    int nextValue;
    switch (selectedMenuEntry)
    {
    case CHANNEL:
        selectedMenuChannel--;
        if(selectedMenuChannel<0) selectedMenuChannel = SUPPORTED_CHANNELS-1;
        break;
    case FUNCTION:
        nextValue = static_cast<int>(radioData.functionToChannelData.functionOnChannel[selectedMenuChannel]);
        nextValue--;
        if(nextValue < 0) nextValue = RadioData::NUMBER_OF_FUNCTIONS-1;
        radioData.functionToChannelData.functionOnChannel[selectedMenuChannel] = static_cast<RadioData::Function>(nextValue);
        break;
    case INVERT:
        radioData.functionToChannelData.invertChannel[selectedMenuChannel] = !radioData.functionToChannelData.invertChannel[selectedMenuChannel];
        break;
    case LOWER:
        radioData.functionToChannelData.lowerLimitChannel[selectedMenuChannel]-=stepSize;
        if(radioData.functionToChannelData.lowerLimitChannel[selectedMenuChannel] < 0) radioData.functionToChannelData.lowerLimitChannel[selectedMenuChannel] = 0;
        break;
    case UPPER:
        radioData.functionToChannelData.upperLimitChannel[selectedMenuChannel]-=stepSize;
        if(radioData.functionToChannelData.upperLimitChannel[selectedMenuChannel] < 0) radioData.functionToChannelData.upperLimitChannel[selectedMenuChannel] = 0;
        if(radioData.functionToChannelData.upperLimitChannel[selectedMenuChannel] <= radioData.functionToChannelData.lowerLimitChannel[selectedMenuChannel]) radioData.functionToChannelData.upperLimitChannel[selectedMenuChannel]+=stepSize;
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}

bool FunctionToChannel::right()
{
    int nextValue;
    switch (selectedMenuEntry)
    {
    case CHANNEL:
        selectedMenuChannel++;
        if(selectedMenuChannel>=SUPPORTED_CHANNELS) selectedMenuChannel = 0;
        break;
    case FUNCTION:
        nextValue = static_cast<int>(radioData.functionToChannelData.functionOnChannel[selectedMenuChannel]);
        nextValue++;
        if(nextValue >= RadioData::NUMBER_OF_FUNCTIONS) nextValue = 0;
        radioData.functionToChannelData.functionOnChannel[selectedMenuChannel] = static_cast<RadioData::Function>(nextValue);        
        break;
    case INVERT:
        radioData.functionToChannelData.invertChannel[selectedMenuChannel] = !radioData.functionToChannelData.invertChannel[selectedMenuChannel];
        break;
    case LOWER:
        radioData.functionToChannelData.lowerLimitChannel[selectedMenuChannel]+=stepSize;
        if(radioData.functionToChannelData.lowerLimitChannel[selectedMenuChannel] > 2047) radioData.functionToChannelData.lowerLimitChannel[selectedMenuChannel] = 2047;
        if(radioData.functionToChannelData.lowerLimitChannel[selectedMenuChannel] >= radioData.functionToChannelData.upperLimitChannel[selectedMenuChannel]) radioData.functionToChannelData.lowerLimitChannel[selectedMenuChannel]-=stepSize;
        break;
    case UPPER:
        radioData.functionToChannelData.upperLimitChannel[selectedMenuChannel]+=stepSize;
        if(radioData.functionToChannelData.upperLimitChannel[selectedMenuChannel] > 2047) radioData.functionToChannelData.upperLimitChannel[selectedMenuChannel] = 2047;
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}

void FunctionToChannel::center()
{

}


#endif