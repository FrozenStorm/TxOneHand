#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <TFT_eSPI.h>

class Display
{
private:
    
public:
    Display()
    static TFT_eSPI& Display::getInstance()
    {
        static TFT_eSPI obj;
        return obj;
    }
};

void Display::Display()
{
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0, 2);
    tft.setTextColor(TFT_WHITE,TFT_BLACK,true);  
    tft.setTextSize(1);
}



#endif