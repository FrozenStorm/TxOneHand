#ifndef DUAL_RATE_HPP
#define DUAL_RATE_HPP

#include "RadioClass.hpp"

class DualRate : public RadioClass
{
private:
    double rateStickLeftRight = 1;
    double rateStickUpDown = 1;
    double rateSlider = 1;
    void calcRate(double& value, double rate);
public:
    DualRate(TFT_eSPI& newTft, DigitalValues& newDigitalValues) : RadioClass(newTft, newDigitalValues){}
    void doFunction();

    void showMenu();
    void showValue();
    void showTitle();
    void up();
    void down();
    void left();
    void right();
    void center();
};


void DualRate::showValue(){};
void DualRate::showTitle(){};
void DualRate::center(){};

void DualRate::showMenu()
{
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(15, 140, 2);
    tft.setTextColor(TFT_WHITE,TFT_BLACK);  
    tft.setTextSize(1);

    tft.println("TxOneHand by Z-Craft");
    tft.println();

    tft.println("Dual Rate");

    tft.printf("rateStickLeftRight = %.0f%%\n",rateStickLeftRight*100);

    tft.printf("rateStickUpDown = %.0f%%\n",rateStickUpDown*100);
    
    tft.printf("rateSlider = %.0f%%\n",rateSlider*100);
}

void DualRate::up()
{
    rateStickUpDown+=0.05;
    limitValue(rateStickUpDown);
}
void DualRate::down()
{
    rateStickUpDown-=0.05;
    if(rateStickUpDown < 0 ) rateStickUpDown = 0;
}
void DualRate::left()
{
    rateStickLeftRight-=0.05;
    if(rateStickLeftRight < 0 ) rateStickLeftRight = 0;
}
void DualRate::right()
{
    rateStickLeftRight+=0.05;
    limitValue(rateStickLeftRight);
}

void DualRate::doFunction()
{
    calcRate(digitalValues.slider, rateSlider);
    calcRate(digitalValues.stickLeftRight, rateStickLeftRight);
    calcRate(digitalValues.stickUpDown, rateStickUpDown);
}

void DualRate::calcRate(double& value, double rate)
{
    if(value > rate) value = rate;
    if(value < -rate) value = -rate;
}


#endif