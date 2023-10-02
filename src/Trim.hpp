#ifndef TRIM_HPP
#define TRIM_HPP

#include "RadioClass.hpp"

class Trim : public RadioClass
{
private:
    void calcTrim(double& value, const int trim);
    void changeTrim(int& trim, const bool moreNotLess);
    void drawTrimH(int posW, int posH, int lenW, int lenH, double value);
    void drawTrimV(int posW, int posH, int lenW, int lenH, double value);
    void drawStick(int posW, int posH, int rad, double valueW, double valueH);
    void drawSlider(int posW, int posH, int lenW, int lenH, double value);
public:
    Trim(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData){}
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

void Trim::showValue(){};
const char * Trim::getTitle()
{
    return "Trim";
}

void Trim::doFunction()
{
    calcTrim(radioData.functionData.roll,radioData.trimData.roll);
    calcTrim(radioData.functionData.pitch,radioData.trimData.pitch);
    calcTrim(radioData.functionData.throttle,radioData.trimData.slider);
}

void Trim::calcTrim(double& value, const int trim)
{
    value += trim * radioData.trimData.stepSize;
    limitValue(value);
}

void Trim::up() 
{
    changeTrim(radioData.trimData.pitch, true);
}

void Trim::down()
{
    changeTrim(radioData.trimData.pitch, false);
}

void Trim::left()
{
    changeTrim(radioData.trimData.roll, false);
}

void Trim::right()
{
    changeTrim(radioData.trimData.roll, true);
}

void Trim::center()
{

}

void Trim::changeTrim(int& trim, const bool moreNotLess)
{
    if(moreNotLess == true && trim < radioData.trimData.stepCountLimit)
    {
        trim++;
    }
    if(moreNotLess == false && trim > -radioData.trimData.stepCountLimit)
    {
        trim--;
    }
}

void Trim::showMenu()
{
    drawStick(tft.width()/2, tft.height()/2+90, 45, radioData.functionData.roll, radioData.functionData.pitch);
    drawTrimH(tft.width()/2-45, tft.height()/2+145, 90, 10, radioData.trimData.roll);
    drawTrimV(tft.width()/2-50-20, tft.height()/2+45, 10, 90, radioData.trimData.pitch);
    drawSlider(tft.width()/2+50+10, tft.height()/2+45, 20, 110, radioData.functionData.throttle);
}

void Trim::drawTrimH(int posW, int posH, int lenW, int lenH, double value){
  static double valueLast = 0;
  double pos = 0;
  pos = posW+lenW/2+valueLast/radioData.trimData.stepCountLimit*lenW/2;
  tft.drawLine(pos,posH,pos,posH+lenH-1,TFT_BLACK);
  tft.drawRect(posW, posH, lenW, lenH,TFT_WHITE);
  tft.drawLine(posW+lenW/2,posH,posW+lenW/2,posH+lenH-1,TFT_WHITE);
  pos = posW+lenW/2+value/radioData.trimData.stepCountLimit*lenW/2;
  tft.drawLine(pos,posH,pos,posH+lenH-1,TFT_RED);
  valueLast = value;
} 

void Trim::drawTrimV(int posW, int posH, int lenW, int lenH, double value){
  static double valueLast = 0;
  double pos = 0;
  pos = posH+lenH/2-valueLast/radioData.trimData.stepCountLimit*lenH/2;
  tft.drawLine(posW,int(pos),posW+lenW-1,int(pos),TFT_BLACK);
  tft.drawRect(posW, posH, lenW, lenH, TFT_WHITE);
  tft.drawLine(posW,posH+lenH/2,posW+lenW-1,posH+lenH/2,TFT_WHITE);
  pos = posH+lenH/2-value/radioData.trimData.stepCountLimit*lenH/2;
  tft.drawLine(posW,int(pos),posW+lenW-1,int(pos),TFT_RED);
  valueLast = value;
}

void Trim::drawStick(int posW, int posH, int rad, double valueW, double valueH){
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

void Trim::drawSlider(int posW, int posH, int lenW, int lenH, double value){
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