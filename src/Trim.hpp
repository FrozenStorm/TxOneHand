#ifndef TRIM_HPP
#define TRIM_HPP

#include "RadioClass.hpp"

class Trim : public RadioClass
{
private:
    float stepSize = 0.01;
    int stepCountLimit = 20;
    void calcTrim(float& value, const int trim);
    void changeTrim(int& trim, const bool moreNotLess);
    void drawTrimH(int posW, int posH, int lenW, int lenH, float value);
    void drawTrimV(int posW, int posH, int lenW, int lenH, float value);
    void drawStick(int posW, int posH, int rad, float valueW, float valueH);
    void drawSlider(int posW, int posH, int lenW, int lenH, float value);
public:
    Trim(RadioData& newRadioData) : RadioClass(newRadioData){}
    void doFunction() override;
};

void Trim::doFunction()
{
    calcTrim(radioData.functionData.roll,radioData.trimData.roll);
    calcTrim(radioData.functionData.pitch,radioData.trimData.pitch);
    calcTrim(radioData.functionData.throttle,radioData.trimData.throttle);
}

void Trim::calcTrim(float& value, const int trim)
{
    value += trim * stepSize;
    limitValue(value);
}

void Trim::changeTrim(int& trim, const bool moreNotLess)
{
    if(moreNotLess == true && trim < stepCountLimit)
    {
        trim++;
    }
    if(moreNotLess == false && trim > -stepCountLimit)
    {
        trim--;
    }
    radioData.storeTrimData();
}

#endif