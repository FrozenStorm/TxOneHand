#ifndef DIGITAL_VALUES_HPP
#define DIGITAL_VALUES_HPP

class DigitalValues
{
private:
    /* data */
public:
    double stickUpDown=0;
    double stickLeftRight=0;
    double slider=0;

    double batteryVoltage=0;

    bool switchPos=0;

    bool leftValue=0;
    bool leftEvent=0;
    bool rightValue=0;
    bool rightEvent=0;
    bool upValue=0;
    bool upEvent=0;
    bool downValue=0;
    bool downEvent=0;
    bool centerValue=0;
    bool centerEvent=0;
    DigitalValues(/* args */);
    ~DigitalValues();
};

DigitalValues::DigitalValues(/* args */)
{
}

DigitalValues::~DigitalValues()
{
}

#endif