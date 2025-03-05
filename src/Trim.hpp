#ifndef TRIM_HPP
#define TRIM_HPP

#include "RadioClass.hpp"

class Trim : public RadioClass
{
public:
    Trim(RadioData& newRadioData) : RadioClass(newRadioData){}
    void doFunction() override;
};

void Trim::doFunction()
{
    radioData.functionData.roll += radioData.trimData.roll;
    radioData.functionData.pitch += radioData.trimData.pitch;
}

#endif