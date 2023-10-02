#ifndef DIGITAL_TO_FUNCTION_HPP
#define DIGITAL_TO_FUNCTION_HPP

#include "RadioClass.hpp"


class DigitalToFunction : public RadioClass
{
private:
public:
    DigitalToFunction(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData){}
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

void DigitalToFunction::showValue(){};
const char * DigitalToFunction::getTitle()
{
    return "DigitalToFunction";
}

void DigitalToFunction::doFunction()
{
    radioData.functionData.pitch = radioData.digitalData.stickUpDown;
    radioData.functionData.roll = radioData.digitalData.stickLeftRight;
    radioData.functionData.throttle = radioData.digitalData.slider;
}


void DigitalToFunction::up() 
{

}

void DigitalToFunction::down()
{

}

void DigitalToFunction::left()
{

}

void DigitalToFunction::right()
{

}

void DigitalToFunction::center()
{

}


void DigitalToFunction::showMenu()
{

}


#endif