#ifndef RADIO_DATA_HPP
#define RADIO_DATA_HPP

#include <EEPROM.h>
#include <list.h>
#include <string.h>
#include "Protocol.hpp"

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

    std::vector<Protocol> protocolList = {
        {0,"NO RF",{        // NO RF
            {0,"NO RF"}
            }},
        {1,"FLYSKY",{            // =>A7105
            {0,"Flysky"},
            {1,"V9X9"},
            {2,"V6X6"},
            {3,"V912"},
            {4,"CX20"}
            }},
        {2,"HUBSAN",{            // =>A7105
            {0,"H107"},
            {1,"H301"},
            {2,"H501"}
            }},
        {3,"FRSKYD",{            // =>CC2500
            {0,"FRSKYD"},
            {1,"DCLONE"}
            }},
        {4,"HISKY",{             // =>NRF24L01
            {0,"Hisky"},
            {1,"HK310"}
            }},
        {5,"V2X2",{              // =>NRF24L01
            {0,"V2X2"},
            {1,"JXD506"},
            {2,"V2X2_MR101"}
            }},
        {6,"DSM",{               // =>CYRF6936
            {0,"DSM2_1F"},
            {1,"DSM2_2F"},
            {2,"DSMX_1F"},
            {3,"DSMX_2F"},
            {4,"DSM_AUTO"},
            {5,"DSMR"}
            }},
        {7,"DEVO",{              // =>CYRF6936
            {0,"NO SUB"}
            }},
        {8,"YD717",{             // =>NRF24L01
            {0,"YD717"},
            {1,"SKYWLKR"},
            {2,"SYMAX4"},
            {3,"XINXUN"},
            {4,"NIHUI"}
            }},
        {9,"KN",{                // =>NRF24L01
            {0,"WLTOYS"},
            {1,"FEILUN"}
            }},
        {10,"SYMAX",{            // =>NRF24L01
            {0,"SYMAX"},
            {1,"SYMAX5C"}
            }},
        {11,"SLT",{              // =>NRF24L01
            {0,"SLT_V1"},
            {1,"SLT_V2"},
            {2,"Q100"},
            {3,"Q200"},
            {4,"MR100"}
            }},
        {12,"CX10",{             // =>NRF24L01
            {0,"CX10_GREEN"},
            {1,"CX10_BLUE"},	// also compatible with CX10-A, CX12
            {2,"DM007"},
            {4,"JC3015_1"},
            {5,"JC3015_2"},
            {6,"MK33041"}
            }},
        {13,"CG023",{            // =>NRF24L01
            {0,"CG023"},
            {1,"YD829"}
            }},
        {14,"BAYANG",{           // =>NRF24L01
            {0,"BAYANG"},
            {1,"H8S3D"},
            {2,"X16_AH"},
            {3,"IRDRONE"},
            {4,"DHD_D4"},
            {5,"QX100"}
            }},
        {15,"FRSKYX",{           // =>CC2500
            {0,"CH_16"},
            {1,"CH_8"},
            {2,"EU_16"},
            {3,"EU_8"},
            {4,"XCLONE_16"},
            {5,"XCLONE_8"}
            }},
        {16,"ESKY",{             // =>NRF24L01
            {0,"ESKY_STD"},
            {1,"ESKY_ET4"}
            }},
        {17,"MT99XX",{           // =>NRF24L01
            {0,"MT99"},
            {1,"H7"},
            {2,"YZ"},
            {3,"LS"},
            {4,"FY805"},
            {5,"A180"},
            {6,"DRAGON"},
            {7,"F949G"}
            }},
        {18,"MJXQ",{             // =>NRF24L01
            {0,"WLH08"},
            {1,"X600"},
            {2,"X800"},
            {3,"H26D"},
            {4,"E010"},
            {5,"H26WH"},
            {6,"PHOENIX"}
            }},
        {19,"SHENQI",{           // =>NRF24L01
            {0,"NO SUB"}
            }},
        {20,"FY326",{            // =>NRF24L01
            {0,"FY326"},
            {1,"FY319"}
            }},
        {21,"FUTABA",{           // =>CC2500
            {0,"NO SUB"}
            }},
        {22,"J6PRO",{            // =>CYRF6936
            {0,"NO SUB"}
            }},
        {23,"FQ777",{            // =>NRF24L01
            {0,"NO SUB"}
            }},
        {24,"ASSAN",{            // =>NRF24L01
            {0,"NO SUB"}
            }},
        {25,"FRSKYV",{           // =>CC2500
            {0,"NO SUB"}
            }},
        {26,"HONTAI",{           // =>NRF24L01
            {0,"HONTAI"},
            {1,"JJRCX1"},
            {2,"X5C1"},
            {3,"FQ777_951"},
            }},
        {27,"OPENLRS",{          // =>OpenLRS hardware
            {0,"NO SUB"}
            }},
        {28,"AFHDS2A",{          // =>A7105
            {0,"NO SUB"}
            }},
        {29,"Q2X2",{             // =>NRF24L01, extension of CX-10 protocol
            {0,"Q222"},
            {1,"Q242"},
            {2,"Q282"},
            {8,"F_Q222"},
            {9,"F_Q242"},
            {0,"F_Q282"}
            }},
        {30,"WK2x01",{           // =>CYRF6936
            {0,"WK2801"},
            {1,"WK2401"},
            {2,"W6_5_1"},
            {3,"W6_6_1"},
            {4,"W6_HEL"},
            {5,"W6_HEL_I"}
            }},
        {31,"Q303",{             // =>NRF24L01
            {0,"Q303"},
            {1,"CX35"},
            {2,"CX10D"},
            {3,"CX10WD"}
            }},
        {32,"GW008",{            // =>NRF24L01
            {0,"NO SUB"}
            }},
        {33,"DM002",{            // =>NRF24L01
            {0,"NO SUB"}
            }},
        {34,"CABELL",{           // =>NRF24L01
            {0,"CABELL_V3"},
            {1,"CABELL_V3_TELEMETRY"},
            {6,"CABELL_SET_FAIL_SAFE"},
            {7,"CABELL_UNBIND"}
            }},
        {35,"ESKY150",{          // =>NRF24L01
            {0,"ESKY150_4CH"},
            {1,"ESKY150_7CH"}
            }},
        {36,"H8_3D",{            // =>NRF24L01
            {0,"H8_3D"},
            {1,"H20H"},
            {2,"H20MINI"},
            {3,"H30MINI"}
            }},
        {37,"CORONA",{           // =>CC2500
            {0,"COR_V1"},
            {1,"COR_V2"},
            {2,"FD_V3"}
            }},
        {38,"CFLIE",{            // =>NRF24L01
            {0,"NO SUB"}
            }},
        {39,"HITEC",{            // =>CC2500
            {0,"OPT_FW"},
            {1,"OPT_HUB"},
            {2,"MINIMA"}
            }},
        {40,"WFLY",{             // =>CYRF6936
            {0,"NO SUB"}
            }},
        {41,"BUGS",{             // =>A7105
            {0,"NO SUB"}
            }},
        {42,"BUGSMINI",{         // =>NRF24L01
            {0,"BUGSMINI"},
            {1,"BUGS3H"},
            }},
        {43,"TRAXXAS",{          // =>CYRF6936
            {0,"RX6519"}
            }},
        {44,"NCC1701",{          // =>NRF24L01
            {0,"NO SUB"}
            }},
        {45,"E01X",{             // =>CYRF6936
            {0,"E012"},
            {1,"E015"},
            {2,"E016H"}
            }},
        {46,"V911S",{            // =>NRF24L01
            {0,"V911S_STD"},
            {1,"V911S_E119"}
            }},
        {47,"GD00X",{            // =>NRF24L01
            {0,"GD_V1"},
            {1,"GD_V2"}
            }},
        {48,"V761",{             // =>NRF24L01
            {0,"V761_3CH"},
            {1,"V761_4CH"},
            {2,"V761_TOPRC"}
            }},
        {49,"KF606",{            // =>NRF24L01
            {0,"KF606_KF606"},
            {1,"KF606_MIG320"},
            {2,"KF606_ZCZ50"}
            }},
        {50,"REDPINE",{          // =>CC2500
            {0,"RED_FAST"},
            {1,"RED_SLOW"}
            }},
        {51,"POTENSIC",{         // =>NRF24L01
            {0,"NO SUB"}
            }},
        {52,"ZSX",{              // =>NRF24L01
            {0,"NO SUB"}
            }},
        {53,"HEIGHT",{           // =>A7105   
            {0,"HEIGHT_5CH"},
            {1,"HEIGHT_8CH"}
            }},
        {54,"SCANNER",{          // =>CC2500
            {0,"NO SUB"}
            }},
        {55,"FRSKY_RX",{         // =>CC2500
            {0,"FRSKY_RX"},
            {1,"FRSKY_CLONE"},
            {2,"FRSKY_ERASE"},
            {3,"FRSKY_CPPM"}
            }},
        {56,"ROTO_AFHDS2A_RX",{        // =>A7105
            {0,"PWM_IBUS"},
            {1,"PPM_IBUS"},
            {2,"PWM_SBUS"},
            {3,"PPM_SBUS"},
            {4,"PWM_IB16"},
            {5,"PPM_IB16"},
            {6,"PWM_SB16"},
            {7,"PPM_SB16"}
            }},
        {57,"HOTT",{             // =>CC2500
            {0,"HOTT_SYNC"},
            {1,"HOTT_NO_SYNC"}
            }},
        {58,"FX",{               // =>NRF24L01
            {0,"FX816"},
            {1,"FX620"},
            {2,"FX9630"}
            }},
        {59,"BAYANG_RX",{        // =>NRF24L01
            {0,"NO SUB"}
            }},
        {60,"PELIKAN",{          // =>A7105
            {0,"PELIKAN_PRO"},
            {1,"PELIKAN_LITE"},
            {2,"PELIKAN_SCX24"}
            }},
        {61,"TIGER",{            // =>NRF24L01
            {0,"NO SUB"}
            }},
        {62,"XK",{               // =>NRF24L01
            {0,"X450"},
            {1,"X420"}
            }},
        {63,"XN297DUMP",{        // =>NRF24L01
            {0,"XN297DUMP_250K"},
            {1,"XN297DUMP_1M"},
            {2,"XN297DUMP_2M"},
            {3,"XN297DUMP_AUTO"},
            {4,"XN297DUMP_NRF"},
            {5,"XN297DUMP_CC2500"}
            }},
        {64,"FRSKYX2",{          // =>CC2500
            {0,"NO SUB"}
            }},
        {65,"FRSKY_R9",{         // =>SX1276
            {0,"R9_915"},
            {1,"R9_868"},
            {2,"R9_915_8CH"},
            {3,"R9_868_8CH"},
            {4,"R9_FCC"},
            {5,"R9_EU"},
            {6,"R9_FCC_8CH"},
            {7,"R9_EU_8CH"}
            }},
        {66,"PROPEL",{           // =>NRF24L01
            {0,"NO SUB"}
            }},
        {67,"FRSKYL",{           // =>CC2500
            {0,"LR12"},
            {1,"LR12_6CH"}
            }},
        {68,"SKYARTEC",{         // =>CC2500
            {0,"NO SUB"}
            }},
        {69,"ESKY150V2",{        // =>CC2500+NRF24L01
            {0,"NO SUB"}
            }},
        {70,"DSM_RX",{           // =>CYRF6936
            {0,"NO SUB"}
            }},
        {71,"JJRC345",{          // =>NRF24L01
            {0,"JJRC345"},
            {1,"SKYTMBLR"}
            }},
        {72,"Q90C",{             // =>NRF24L01 or CC2500
            {0,"NO SUB"}
            }},
        {73,"KYOSHO",{           // =>A7105
            {0,"KYOSHO_FHSS"},
            {1,"KYOSHO_HYPE"}
            }},
        {74,"RLINK",{            // =>CC2500
            {0,"RLINK_SURFACE"},
            {1,"RLINK_AIR"},
            {2,"RLINK_DUMBORC"}
            }},
        {76,"REALACC",{          // =>NRF24L01
            {0,"NO SUB"}
            }},
        {77,"OMP",{              // =>CC2500 & NRF24L01
            {0,"NO SUB"}
            }},
        {78,"MLINK",{            // =>CYRF6936
            {0,"NO SUB"}
            }},
        {79,"WFLY2",{            // =>A7105
            {0,"NO SUB"}
            }},
        {80,"E016HV2",{          // =>CC2500 & NRF24L01
            {0,"NO SUB"}
            }},
        {81,"E010R5",{           // =>CYRF6936
            {0,"NO SUB"}
            }},
        {82,"LOLI",{             // =>NRF24L01
            {0,"NO SUB"}
            }},
        {83,"E129",{             // =>CYRF6936
            {0,"E129_E129"},
            {1,"E129_C186"}
            }},
        {84,"JOYSWAY",{          // =>A7105
            {0,"NO SUB"}
            }},
        {85,"E016H",{            // =>NRF24L01
            {0,"NO SUB"}
            }},
        {86,"CONFIG",{           // Module config
            {0,"NO SUB"}
            }},
        {87,"IKEAANSLUTA",{      // =>CC2500
            {0,"NO SUB"}
            }},
        {88,"WILLIFM",{          // 27/35ab/40/41/72 MHz module external project
            {0,"NO SUB"}
            }},
        {89,"LOSI",{             // =>CYRF6936
            {0,"NO SUB"}
            }},
        {90,"MOULDKG",{          // =>NRF24L01
            {0,"MOULDKG_ANALOG"},
            {1,"MOULDKG_DIGIT"}
            }},
        {91,"XERALL",{           // =>NRF24L01
            {0,"NO SUB"}
            }},
        {92,"MT99XX2",{          // =>NRF24L01, extension of MT99XX protocol
            {0,"PA18"}
            }},
        {93,"KYOSHO2",{          // =>NRF24L01
            {0,"NO SUB"}
            }},
        {126,"NANORF",{          // =>NRF24L01
            {0,"NO SUB"}
            }},
        {127,"TEST",{            // =>CC2500
            {0,"NO SUB"}
            }}
    };

    enum BindingState{ BINDED, BINDING_STARTED, BINDING, BINDING_FINISHED, BINDING_FAILED, NUMBER_OF_BINDING_STATES};
    const char* bindingStateNames[NUMBER_OF_BINDING_STATES] = {"Binded", "Binding started", "Binding", "Binding finished", "Binding failed"};

    struct TransmitterData
    {
        BindingState bindingState;
        unsigned int selectedProtocol;
        unsigned int selectedSubProtocol;
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
        .selectedProtocol = 6,
        .selectedSubProtocol = 2,
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