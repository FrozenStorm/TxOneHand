#ifndef DIGITAL_TO_FUNCTION_HPP
#define DIGITAL_TO_FUNCTION_HPP

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
    if(radioData.functionData.throttle > 0.5 && radioData.functionData.armed == 0 &&
        radioData.digitalData.trim == 1 && radioData.digitalData.armEvent == 1)
    {
        if(radioData.transmitterData.bindingState == radioData.BINDED || radioData.transmitterData.bindingState == radioData.BINDING_FAILED){
            radioData.transmitterData.bindingState = radioData.BINDING_STARTED;
        }
    }

    // Trim
    if(radioData.digitalData.trimLongPressEvent == 1){
        radioData.trimData.pitch = radioData.digitalData.pitch;
        radioData.trimData.roll = radioData.digitalData.roll;
        radioData.sensorToDigitalData.angleLimitPitch.center = radioData.analogData.pitch;
        radioData.sensorToDigitalData.angleLimitRoll.center = radioData.analogData.roll;
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
        radioData.functionData.throttle = sqrt(radioData.digitalData.stickUpDown * radioData.digitalData.stickUpDown + radioData.digitalData.stickLeftRight * radioData.digitalData.stickLeftRight);
    }
    else
    {
        radioData.functionData.pitch = 0;
        radioData.functionData.roll = 0;
        radioData.functionData.throttle = 0;
    } 

    // Batterie
    if(radioData.analogData.battery > 3.8)
    {
        digitalWrite(PIN_LED,1);
    }
    else{
        digitalWrite(PIN_LED,0);
    }
}
#endif