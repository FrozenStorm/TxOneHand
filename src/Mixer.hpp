#ifndef MIXER_HPP
#define MIXER_HPP

#include "RadioClass.hpp"

class Mixer : public RadioClass
{
private:

public:
    Mixer(RadioData& newRadioData) : RadioClass(newRadioData){}
    void doFunction();
};

void Mixer::doFunction()
{
    radioData.functionData.pitch += radioData.mixerData.throttleToPitch * ((radioData.functionData.throttle + 1) / 2);
}



#endif