#ifndef RADIO_DATA_HPP
#define RADIO_DATA_HPP

#include <EEPROM.h>

class RadioData
{
private:
    int eepromSize = 0;
public:
   
    struct AnalogToDigitalData
    {
        struct StickLimit{
            double min;
            double max;
            double center;
            bool invert;
        };
        StickLimit stickLimitUpDown;
        StickLimit stickLimitLeftRight;
        StickLimit stickLimitSlider;

        struct MenuButtonLimit
        {
            double up;
            double down;
            double left;
            double right;
            double center;
        };
        MenuButtonLimit menuButtonLimit;
        double menuButtonTolerance;
        long longPressDurationMs;

        unsigned long startPressTimeUpMs;
        unsigned long startPressTimeDownMs;
        unsigned long startPressTimeLeftMs;
        unsigned long startPressTimeRightMs;
        unsigned long startPressTimeCenterMs;
    };
    AnalogToDigitalData analogToDigitalData;
    

    struct ExpoData{
        double roll;
        double pitch;
        double throttle;
        double stepSize;
    };
    ExpoData expoData;

    struct DualRateData
    {
        double roll;
        double pitch;
        double throttle;
        double stepSize;
    };
    DualRateData dualRateData;

    struct TrimData
    {
        int roll;
        int pitch;
        int slider;
        double stepSize;
        int stepCountLimit;
    };
    TrimData trimData;

    struct MixerData
    {
        double throttleToPitch;
        double stepSize;
    };
    MixerData mixerData;

    enum Function{ NONE, PITCH, ROLL, VTAIL_LEFT, VTAIL_RIGHT, THROTTLE, NUMBER_OF_FUNCTIONS};
    const char* functionNames[NUMBER_OF_FUNCTIONS] = {"NONE", "PITCH", "ROLL", "VTAIL_LEFT", "VTAIL_RIGHT", "THROTTLE"};

    struct FunctionToChannelData
    {
        bool invertChannel[16];
        Function functionOnChannel[16];
        int upperLimitChannel[16];
        int lowerLimitChannel[16];
        int stepSize;
    };
    FunctionToChannelData functionToChannelData;

    struct RawData
    {
        double stickUpDown = 0;
        double stickLeftRight = 0;
        double slider = 0;
        double menu = 0;
        double battery = 0;
    };
    RawData rawData;

    struct AnalogData
    {
        double stickUpDown = 0;
        double stickLeftRight = 0;
        double slider = 0;
        double menu = 0;
        double battery = 0; 
    };
    AnalogData analogData;
    
    struct DigitalData
    {
        double stickUpDown = 0;
        double stickLeftRight = 0;
        double slider = 0;

        bool sideSwitch = 0;

        bool left = 0;
        bool right = 0;
        bool up = 0;
        bool down = 0;
        bool center = 0;
        
        bool leftEvent = 0;
        bool rightEvent = 0;
        bool upEvent = 0;
        bool downEvent = 0;
        bool centerEvent = 0;

        bool leftLongPressEvent = 0;
        bool rightLongPressEvent = 0;
        bool upLongPressEvent = 0;
        bool downLongPressEvent = 0;
        bool centerLongPressEvent = 0;
    };
    DigitalData digitalData;

    struct FunctionData
    {
        double pitch = 0;
        double roll = 0;
        double throttle = 0;
    };
    FunctionData functionData;
    
    struct ChannelData
    {
        int channel[16] = {0};
    };
    ChannelData channelData;

    enum BindingState{ BINDED, BINDING_STARTED, BINDING, BINDING_FINISHED, BINDING_FAILED, NUMBER_OF_BINDING_STATES};
    const char* bindingStateNames[NUMBER_OF_BINDING_STATES] = {"Binded", "Binding started", "Binding", "Binding finished", "Binding failed"};

    struct TransmitterData
    {
        BindingState bindingState;
        unsigned char txData[27]; // TODO wiso muss hier unsigend char stehen, damit das init mit 0xE3 funktioniert
        unsigned char rxData[28];
    };
    TransmitterData transmitterData;


    void storeData();
    void loadData();
    
    RadioData(/* args */);
    ~RadioData();
};

RadioData::RadioData(/* args */)
{
    analogToDigitalData = {
        .stickLimitUpDown = {.min = 0.01, .max = 3.01, .center = 1.34, .invert = true},
        .stickLimitLeftRight = {.min = 0.13, .max = 2.93, .center = 1.5, .invert = false},
        .stickLimitSlider = {.min = 0.25, .max = 1.45, .center = 0.85, .invert = true},
        .menuButtonLimit = {.up = 1.67, .down = 0.53, .left = 0.38, .right = 1.38, .center = 0.96},
        .menuButtonTolerance = 0.1,
        .longPressDurationMs = 600,
        .startPressTimeUpMs = 0,
        .startPressTimeDownMs = 0,
        .startPressTimeLeftMs = 0,
        .startPressTimeRightMs = 0,
        .startPressTimeCenterMs = 0
    };
    expoData = {
        .roll = 0.3,
        .pitch = 0.3,
        .throttle = 0,
        .stepSize = 0.05
    };
    dualRateData = {
        .roll = 1,
        .pitch = 1,
        .throttle = 1,
        .stepSize = 0.05
    };
    trimData = {
        .roll = 0,
        .pitch = 0,
        .slider = 0,
        .stepSize = 0.01,
        .stepCountLimit = 20
    };
    mixerData = {
        .throttleToPitch = 0,
        .stepSize = 0.05
    };
    functionToChannelData = {
        .invertChannel = {false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false},
        .functionOnChannel = {VTAIL_LEFT, VTAIL_RIGHT, THROTTLE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, NONE, THROTTLE, NONE, NONE},
        .upperLimitChannel = {2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047,2047},
        .lowerLimitChannel = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        .stepSize = 20
    };
    rawData = {};
    analogData = {};
    digitalData = {};
    functionData = {};
    channelData = {};
    transmitterData = {
        .bindingState = BINDED,
        .txData = {0x55,0x06,0x20,0x07,0x00,0x24,0x20,0x07,0x01,0x08,0x40,0x00,0x02,0x10,0x80,0x00,0x04,0x20,0x00,0x01,0x08,0x40,0x00,0x02,0x10,0x80,0x08},
        .rxData = {0x4D,0x50,0x01,0x18,0x47,0x01,0x03,0x03,0x14,0xE4,0x46,0x21,0x44,0x53,0x4D,0x00,0x4F,0x4D,0x50,0x76,0x58,0x20,0x31,0x46,0x00,0x00,0x00,0x00}
    };

    
    eepromSize += sizeof(AnalogToDigitalData);
    eepromSize += sizeof(ExpoData);
    eepromSize += sizeof(DualRateData);
    eepromSize += sizeof(TrimData);
    eepromSize += sizeof(FunctionToChannelData);
    eepromSize += sizeof(MixerData);
    // eepromSize += sizeof(RawData);
    // eepromSize += sizeof(AnalogData);
    // eepromSize += sizeof(DigitalData);
    // eepromSize += sizeof(FunctionData);
    // eepromSize += sizeof(ChannelData);
}

RadioData::~RadioData()
{
}

void RadioData::storeData()
{
    int address = 0;

    EEPROM.begin(eepromSize);

    EEPROM.put(address,analogToDigitalData);
    address += sizeof(AnalogToDigitalData);

    EEPROM.put(address,expoData);
    address += sizeof(ExpoData);

    EEPROM.put(address,dualRateData);
    address += sizeof(DualRateData);

    EEPROM.put(address,trimData);
    address += sizeof(TrimData);

    EEPROM.put(address,functionToChannelData);
    address += sizeof(FunctionToChannelData);

    EEPROM.put(address,mixerData);
    address += sizeof(MixerData);

    // TODO SAVE BINDING STATE

    //EEPROM.put<RawData>(address,rawData);
    //address += sizeof(RawData);

    //EEPROM.put<AnalogData>(address,analogData);
    //address += sizeof(AnalogData);

    //EEPROM.put<DigitalData>(address,digitalData);
    //address += sizeof(DigitalData);

    //EEPROM.put<FunctionData>(address,functionData);
    //address += sizeof(FunctionData);

    //EEPROM.put<ChannelData>(address,channelData);
    //address += sizeof(ChannelData);
    EEPROM.end();
}

void RadioData::loadData()
{
    int address = 0; 

    EEPROM.begin(eepromSize);

    EEPROM.get(address, analogToDigitalData);
    address += sizeof(AnalogToDigitalData);

    EEPROM.get(address, expoData);
    address += sizeof(ExpoData);

    EEPROM.get(address, dualRateData);
    address += sizeof(DualRateData);

    EEPROM.get(address, trimData);
    address += sizeof(TrimData);

    EEPROM.get(address, functionToChannelData);
    address += sizeof(FunctionToChannelData);  

    EEPROM.get(address, mixerData);
    address += sizeof(MixerData);   

    //EEPROM.get<RawData>(address, rawData);
    //address += sizeof(RawData);

    //EEPROM.get<AnalogData>(address, analogData);
    //address += sizeof(AnalogData);

    //EEPROM.get<DigitalData>(address, digitalData);
    //address += sizeof(DigitalData);

    //EEPROM.get<FunctionData>(address, functionData);
    //address += sizeof(FunctionData);

    //EEPROM.get<ChannelData>(address, channelData);
    //address += sizeof(ChannelData);
    EEPROM.end();
}

#endif