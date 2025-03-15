#ifndef ANALOG_TO_DIGITAL_HPP
#define ANALOG_TO_DIGITAL_HPP

#include "RadioClass.hpp"
#include "esp_adc_cal.h"

#define PIN_ROLL                ADC1_CHANNEL_0 // GPIO01 // ADC1_CH0
#define PIN_PITCH               ADC1_CHANNEL_3 // GPIO04 // ADC1_CH4
#define PIN_VBAT                ADC1_CHANNEL_1 // GPIO02 // ADC1_CH1
#define PIN_LED                 8
#define PIN_VIBRATION           5

#define PIN_TRIM                9
#define PIN_ARM                 11

class AnalogToDigital : public RadioClass
{
private:
    esp_adc_cal_characteristics_t   adc_chars;    
    unsigned int                    changedTimeArmMs = 0;
    unsigned int                    changedTimeTrimMs = 0;
    unsigned int                    startPressTimeArmMs = 0;
    unsigned int                    startPressTimeTrimMs = 0;
    float analogToDigital(float value, const RadioData::AnalogToDigitalData::StickLimit& limit);
    void getButton(const bool& value, unsigned int& changeTimeMs, bool& button, bool& buttonEvent);
    void getLongPress(bool& longPressEvent, unsigned int& startTimeMs, const bool& state, const bool& event);
public:
    AnalogToDigital(RadioData& newRadioData);
    void doFunction();
};

AnalogToDigital::AnalogToDigital(RadioData& newRadioData) : RadioClass(newRadioData)
{
    pinMode(PIN_TRIM,INPUT_PULLUP);
    pinMode(PIN_ARM,INPUT_PULLUP);
    pinMode(PIN_LED,OUTPUT);
    pinMode(PIN_VIBRATION,OUTPUT);

    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_chars);
    // switch (val_type) {
    //     case ESP_ADC_CAL_VAL_EFUSE_TP: Serial.println("Two Point Calibration"); break;
    //     case ESP_ADC_CAL_VAL_EFUSE_VREF: Serial.println("eFUSE Vref Calibration"); break;
    //     case ESP_ADC_CAL_VAL_DEFAULT_VREF: Serial.println("Default Calibration (1100 mV)"); break;
    //     case ESP_ADC_CAL_VAL_EFUSE_TP_FIT: Serial.println("ESP_ADC_CAL_VAL_EFUSE_TP_FIT"); break;
    //     case ESP_ADC_CAL_VAL_MAX: Serial.println("ESP_ADC_CAL_VAL_MAX"); break;
    // }
}

void AnalogToDigital::doFunction()
{
    radioData.rawData.stickUpDown = adc1_get_raw(PIN_PITCH);
    radioData.rawData.stickLeftRight = adc1_get_raw(PIN_ROLL);
    radioData.rawData.battery = adc1_get_raw(PIN_VBAT);

    radioData.analogData.stickUpDown = esp_adc_cal_raw_to_voltage(radioData.rawData.stickUpDown, &adc_chars)/1000.0;
    radioData.analogData.stickLeftRight = esp_adc_cal_raw_to_voltage(radioData.rawData.stickLeftRight, &adc_chars)/1000.0;
    radioData.analogData.battery = 2 * esp_adc_cal_raw_to_voltage(radioData.rawData.battery, &adc_chars)/1000.0;

    radioData.digitalData.stickUpDown = analogToDigital(radioData.analogData.stickUpDown, radioData.analogToDigitalData.stickLimitUpDown);
    radioData.digitalData.stickLeftRight = analogToDigital(radioData.analogData.stickLeftRight, radioData.analogToDigitalData.stickLimitLeftRight);

    getButton(!digitalRead(PIN_ARM),changedTimeArmMs,radioData.digitalData.arm,radioData.digitalData.armEvent);
    getButton(!digitalRead(PIN_TRIM),changedTimeTrimMs,radioData.digitalData.trim,radioData.digitalData.trimEvent);
    getLongPress(radioData.digitalData.armLongPressEvent,startPressTimeArmMs,radioData.digitalData.arm,radioData.digitalData.armEvent);
    getLongPress(radioData.digitalData.trimLongPressEvent,startPressTimeTrimMs,radioData.digitalData.trim,radioData.digitalData.trimEvent);
}

void AnalogToDigital::getButton(const bool& value, unsigned int& changeTimeMs, bool& button, bool& buttonEvent)
{
    bool newButtonState;
    if(value == true) 
    {
        newButtonState = true;
        if(changeTimeMs == 0)
        {
            changeTimeMs = millis();
        }
    }
    else 
    {
        newButtonState = false;
        changeTimeMs = 0;
    }

    if(button == false && newButtonState == true)
    {
        if(millis() - changeTimeMs >= radioData.analogToDigitalData.longPressDurationMs/10)
        {
            buttonEvent = true;
            button = newButtonState;
        }
    }
    else 
    {
        buttonEvent = false;
        button = newButtonState;
    }
}

void AnalogToDigital::getLongPress(bool& longPressEvent, unsigned int& startTimeMs, const bool& state, const bool& event)
{
    if(event == true) 
    {
        startTimeMs = millis();
    }
    if(state == true && ((float)millis() - (float)startTimeMs) >= radioData.analogToDigitalData.longPressDurationMs)
    {
        longPressEvent = true;
        startTimeMs = millis() + 60000;       
    }
    else 
    {
        longPressEvent = false;
    }
}

float AnalogToDigital::analogToDigital(float value, const RadioData::AnalogToDigitalData::StickLimit& limit)
{
    // Umwandeln aller Werte in +/- Werte
    float max = limit.max - limit.center;
    float min = limit.min - limit.center;
    value = value - limit.center;
    // Umwandeln von analog Bereich zu +/- 1
    if(value >= 0){
        if(max != 0){
            value = value / max;
        }
        else{

        }
    }
    else{
        if(min != 0){
            value = value / abs(min);
        }
        else{

        }
    }
    // Invertieren
    if(limit.invert == true){
        value = value * -1;
    }
    // Limitieren auf digital Bereich
    limitValue(value);

    return value;
}

#endif