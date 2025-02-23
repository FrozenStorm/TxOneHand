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
    if(radioData.digitalData.sideSwitch == 1)
    {
        radioData.functionData.pitch = radioData.digitalData.pitch;
        radioData.functionData.roll = radioData.digitalData.roll;
        radioData.functionData.throttle = radioData.digitalData.slider;
    }
    else
    {
        radioData.functionData.pitch = radioData.digitalData.stickUpDown;
        radioData.functionData.roll = radioData.digitalData.stickLeftRight;
        radioData.functionData.throttle = radioData.digitalData.slider;
    }

}
#endif