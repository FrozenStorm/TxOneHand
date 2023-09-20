#ifndef TRIM_HPP
#define TRIM_HPP

#include "RadioClass.hpp"

#define DISP_WIDTH            170
#define DISP_HEIGHT           320

class Trim : public RadioClass
{
private:
    double stepSize = 0.01;
    int maxSteps = 20;
    int trimStickLeftRight = 0;
    int trimStickUpDown = 0;
    int trimSlider = 0;
    void calcTrim(double& value, int trim);
    void changeTrim(int& trim, bool moreNotLess);
    void drawTrimH(uint8_t posW, uint8_t posH, uint8_t lenW, uint8_t lenH, double value);
    void drawTrimV(uint8_t posW, uint8_t posH, uint8_t lenW, uint8_t lenH, double value);
    void drawStick(uint8_t posW, uint8_t posH, uint8_t rad, double valueW, double valueH);
    void drawSlider(uint8_t posW, uint8_t posH, uint8_t lenW, uint8_t lenH, double value);
public:
    Trim(TFT_eSPI& newTft, DigitalValues& newDigitalValues) : RadioClass(newTft, newDigitalValues){}
    void doFunction();
    void showMenu();
    void up();
    void down();
    void left();
    void right();
    void center();
};

void Trim::doFunction()
{
    calcTrim(digitalValues.stickLeftRight,trimStickLeftRight);
    calcTrim(digitalValues.stickUpDown,trimStickUpDown);
    calcTrim(digitalValues.slider,trimSlider);
}

void Trim::calcTrim(double& value, int trim)
{
    value += trim * stepSize;
    limitValue(value);
}

void Trim::up() 
{
    changeTrim(trimStickUpDown, true);
}

void Trim::down()
{
    changeTrim(trimStickUpDown, false);
}

void Trim::left()
{
    changeTrim(trimStickLeftRight, false);
}

void Trim::right()
{
    changeTrim(trimStickLeftRight, true);
}

void Trim::center()
{

}

void Trim::changeTrim(int& trim, bool moreNotLess)
{
    if(moreNotLess == true && trim < maxSteps)
    {
        trim++;
    }
    if(moreNotLess == false && trim > -maxSteps)
    {
        trim--;
    }
}

void Trim::showMenu()
{
    //tft.print("Trim l-r,u|d=");
    //tft.print(trimStickLeftRight);
    //tft.print(",");
    //tft.println(trimStickUpDown);
    drawStick(DISP_WIDTH/2, 60, 50, digitalValues.stickLeftRight, digitalValues.stickUpDown);
    drawTrimH(DISP_WIDTH/2-50, 120, 100, 10, trimStickLeftRight);
    drawTrimV(DISP_WIDTH/2-50-20, 10, 10, 100, trimStickUpDown);
    drawSlider(DISP_WIDTH/2+50+10, 10, 20, 120, digitalValues.slider);
}

void Trim::drawTrimH(uint8_t posW, uint8_t posH, uint8_t lenW, uint8_t lenH, double value){
  static double valueLast = 0;
  double pos = 0;
  pos = posW+lenW/2+valueLast/maxSteps*lenW/2;
  tft.drawLine(pos,posH,pos,posH+lenH-1,TFT_BLACK);
  tft.drawRect(posW, posH, lenW, lenH,TFT_WHITE);
  tft.drawLine(posW+lenW/2,posH,posW+lenW/2,posH+lenH-1,TFT_WHITE);
  pos = posW+lenW/2+value/maxSteps*lenW/2;
  tft.drawLine(pos,posH,pos,posH+lenH-1,TFT_RED);
  valueLast = value;
} 

void Trim::drawTrimV(uint8_t posW, uint8_t posH, uint8_t lenW, uint8_t lenH, double value){
  static double valueLast = 0;
  double pos = 0;
  pos = posH+lenH/2-valueLast/maxSteps*lenH/2;
  tft.drawLine(posW,int(pos),posW+lenW-1,int(pos),TFT_BLACK);
  tft.drawRect(posW, posH, lenW, lenH, TFT_WHITE);
  tft.drawLine(posW,posH+lenH/2,posW+lenW-1,posH+lenH/2,TFT_WHITE);
  pos = posH+lenH/2-value/maxSteps*lenH/2;
  tft.drawLine(posW,int(pos),posW+lenW-1,int(pos),TFT_RED);
  valueLast = value;
}

void Trim::drawStick(uint8_t posW, uint8_t posH, uint8_t rad, double valueW, double valueH){
  static double valueWLast = 0;
  static double valueHLast = 0;

  tft.drawCircle(int(posW+valueWLast*rad), int(posH-valueHLast*rad), int(rad/10), TFT_BLACK);
  // tft.drawCircle(posW, posH, rad, TFT_WHITE);
  tft.drawRect(posW-rad, posH-rad, 2*rad, 2*rad, TFT_WHITE);
  tft.drawLine(posW, posH-rad, posW, posH+rad, TFT_WHITE);
  tft.drawLine(posW-rad, posH, posW+rad, posH, TFT_WHITE);
  tft.drawCircle(int(posW+valueW*rad), int(posH-valueH*rad), int(rad/10), TFT_RED);

  valueWLast = valueW;
  valueHLast = valueH;
}

void Trim::drawSlider(uint8_t posW, uint8_t posH, uint8_t lenW, uint8_t lenH, double value){
  static double valueLast = 0;
  double pos = 0;
  pos = posH+lenH/2-valueLast*lenH/2;
  tft.drawLine(posW,int(pos),posW+lenW-1,int(pos),TFT_BLACK);
  tft.drawRect(posW, posH, lenW, lenH, TFT_WHITE);
  //tft.drawLine(posW,posH+lenH/2,posW+lenW-1,posH+lenH/2,TFT_WHITE);
  pos = posH+lenH/2-value*lenH/2;
  tft.drawLine(posW,int(pos),posW+lenW-1,int(pos),TFT_RED);
  valueLast = value;
}

#endif