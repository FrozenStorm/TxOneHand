#pragma once

#include "RadioClass.hpp"


class DigitalToFunction : public RadioClass
{
private:
public:
    DigitalToFunction(RadioData& newRadioData) : RadioClass(newRadioData){}
    void doFunction() override;
};

void DigitalToFunction::doFunction()
{
    // Binding
    if(radioData.functionData.throttle > 0.5 && radioData.digitalData.trim == 1 && radioData.digitalData.armEvent == 1)
    {
        if(radioData.transmitterData.bindingState == radioData.BINDED || radioData.transmitterData.bindingState == radioData.BINDING_FAILED){
            radioData.transmitterData.bindingState = radioData.BINDING_STARTED;
        }
    }

    // Trim
    if(radioData.digitalData.trimLongPressEvent == 1 && radioData.functionData.armed == 1){
        if(radioData.functionData.pitch != NAN && radioData.functionData.roll != NAN && radioData.analogData.pitch != NAN && radioData.analogData.roll != NAN){
            radioData.trimData.pitch = radioData.functionData.pitch;
            radioData.trimData.roll = radioData.functionData.roll;
            radioData.sensorToDigitalData.angleLimitPitch.center = radioData.analogData.pitch;
            radioData.sensorToDigitalData.angleLimitRoll.center = radioData.analogData.roll;
            radioData.storeTrimData();
        }
    }

    // Arm Switch
    if(radioData.digitalData.armEvent == 1)
    {
        radioData.functionData.armed = !radioData.functionData.armed;
        if(radioData.functionData.armed == 1)
        {
            radioData.sensorToDigitalData.angleLimitPitch.center = radioData.analogData.pitch;
            radioData.sensorToDigitalData.angleLimitRoll.center = radioData.analogData.roll;
        }
    }
    if(radioData.functionData.armed == 1)
    {
        radioData.functionData.pitch = radioData.digitalData.pitch;
        radioData.functionData.roll = radioData.digitalData.roll;
        radioData.functionData.throttle = -1+2*sqrt(radioData.digitalData.stickUpDown * radioData.digitalData.stickUpDown + radioData.digitalData.stickLeftRight * radioData.digitalData.stickLeftRight);
        limitValue(radioData.functionData.throttle);
    }
    else
    {
        radioData.functionData.pitch = 0;
        radioData.functionData.roll = 0;
        radioData.functionData.throttle = -1;
    } 

    // Batterie
    if(radioData.analogData.battery > 3.6)
    {
        digitalWrite(PIN_LED,1);
        digitalWrite(PIN_VIBRATION,0);
    }
    else
    {
        static int slowDown = 0;
        slowDown +=1;
        if(slowDown % 10 == 0) 
        {
            digitalWrite(PIN_LED,digitalRead(PIN_LED) == 1 ? 0 : 1);
            digitalWrite(PIN_VIBRATION,digitalRead(PIN_VIBRATION) == 1 ? 0 : 1);
        }
    }
}