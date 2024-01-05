#ifndef RADIO_DATA_HPP
#define RADIO_DATA_HPP

#include <EEPROM.h>
#include <list.h>
#include "Protocol.hpp"

#define MAX_NUMBER_OF_MODELS 4
#define MODEL_NAME_LENGTH 12
#define CHARACTER_SET_LENGTH 38
#define CRC_VALUE 42
#define SUPPORTED_CHANNELS 8

class RadioData
{
private:
    int eepromSizeModel = 0;
    int eepromSizeGobal = 0;
    char modelNameString[MODEL_NAME_LENGTH+1] = "123456789_12";
    void setDefaultValues(void);
public:
    struct AnalogToDigitalData
    {
        struct StickLimit{
            float min;
            float max;
            float center;
            bool invert;
        };
        StickLimit stickLimitUpDown;
        StickLimit stickLimitLeftRight;
        StickLimit stickLimitSlider;

        struct MenuButtonLimit
        {
            float up;
            float down;
            float left;
            float right;
            float center;
        };
        MenuButtonLimit menuButtonLimit;
        float menuButtonTolerance;
        int longPressDurationMs;
    };
    AnalogToDigitalData analogToDigitalData;
    

    struct ExpoData{
        float roll;
        float pitch;
        float throttle;
    };
    ExpoData expoData;

    struct DualRateData
    {
        float roll;
        float pitch;
        float throttle;
    };
    DualRateData dualRateData;

    struct TrimData
    {
        int roll;
        int pitch;
        int slider;
    };
    TrimData trimData;

    struct MixerData
    {
        float throttleToPitch;
    };
    MixerData mixerData;

    enum Function{ NONE, PITCH, ROLL, VTAIL_LEFT, VTAIL_RIGHT, THROTTLE, NUMBER_OF_FUNCTIONS};
    const char* functionNames[NUMBER_OF_FUNCTIONS] = {"NONE", "PITCH", "ROLL", "VTAIL_LEFT", "VTAIL_RIGHT", "THROTTLE"};

    struct FunctionToChannelData
    {
        bool invertChannel[SUPPORTED_CHANNELS];
        Function functionOnChannel[SUPPORTED_CHANNELS];
        int upperLimitChannel[SUPPORTED_CHANNELS];
        int lowerLimitChannel[SUPPORTED_CHANNELS];
    };
    FunctionToChannelData functionToChannelData;

    struct RawData
    {
        float stickUpDown = 0;
        float stickLeftRight = 0;
        float slider = 0;
        float menu = 0;
        float battery = 0;
    };
    RawData rawData;

    struct AnalogData
    {
        float stickUpDown = 0;
        float stickLeftRight = 0;
        float slider = 0;
        float menu = 0;
        float battery = 0; 
    };
    AnalogData analogData;
    
    struct DigitalData
    {
        float stickUpDown = 0;
        float stickLeftRight = 0;
        float slider = 0;

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
        float pitch = 0;
        float roll = 0;
        float throttle = 0;
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

    enum PowerValue{ POWER_VALUE_HIGH, POWER_VALUE_LOW, NUMBER_OF_POWERVALUES};
    const char* powerValueNames[NUMBER_OF_POWERVALUES] = {"High", "Low"};

    struct TransmitterData
    {
        BindingState bindingState;
        unsigned char selectedProtocol;
        unsigned char selectedSubProtocol;
        bool rangeCheck;
        unsigned char rxNum;
        PowerValue powerValue;
    };
    TransmitterData transmitterData;
    
    const char modelNameCharacters[CHARACTER_SET_LENGTH] = {' ','-','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9'};
    unsigned int selectedModel;
    struct ModelData
    {
        char modelName[MODEL_NAME_LENGTH];
        char crc;
    };
    ModelData modelData;

    void storeData();
    void loadData();

    void storeSelectedModel();
    void loadSelectedModel();

    char* getModelName(void);
    
    RadioData(/* args */);
    ~RadioData();
};

char* RadioData::getModelName(void)
{
    int i;
    bool firstCharacterFound = false;
    for(i=MODEL_NAME_LENGTH-1;i>=0;i--)
    {
        if(modelData.modelName[i] == 0 && firstCharacterFound == false && i != 0){
            modelNameString[i] = '\n';
        }
        else{
            firstCharacterFound = true;
            modelNameString[i] = modelNameCharacters[modelData.modelName[i]];
        }
    }
    return modelNameString;
}

RadioData::RadioData(/* args */)
{
    setDefaultValues();

    eepromSizeModel = 0;
    eepromSizeModel += sizeof(AnalogToDigitalData);
    eepromSizeModel += sizeof(ExpoData);
    eepromSizeModel += sizeof(DualRateData);
    eepromSizeModel += sizeof(TrimData);
    eepromSizeModel += sizeof(FunctionToChannelData);
    eepromSizeModel += sizeof(MixerData);
    eepromSizeModel += sizeof(TransmitterData);
    eepromSizeModel += sizeof(ModelData);
    eepromSizeModel = eepromSizeModel * MAX_NUMBER_OF_MODELS;

    eepromSizeGobal = sizeof(selectedModel);
}

RadioData::~RadioData()
{
}

void RadioData::setDefaultValues()
{
    analogToDigitalData = {
        .stickLimitUpDown = {.min = 0.01, .max = 3.01, .center = 1.34, .invert = true},
        .stickLimitLeftRight = {.min = 0.13, .max = 2.93, .center = 1.5, .invert = false},
        .stickLimitSlider = {.min = 0.25, .max = 1.45, .center = 0.85, .invert = true},
        .menuButtonLimit = {.up = 1.67, .down = 0.53, .left = 0.38, .right = 1.38, .center = 0.96},
        .menuButtonTolerance = 0.1,
        .longPressDurationMs = 600
    };
    expoData = {
        .roll = 0.3,
        .pitch = 0.3,
        .throttle = 0
    };
    dualRateData = {
        .roll = 1,
        .pitch = 1,
        .throttle = 1,
    };
    trimData = {
        .roll = 0,
        .pitch = 0,
        .slider = 0
    };
    mixerData = {
        .throttleToPitch = 0
    };
    functionToChannelData = {
        .invertChannel = {false,false,false,false,false,false,false,false},
        .functionOnChannel = {VTAIL_LEFT, VTAIL_RIGHT, THROTTLE, NONE, NONE, NONE, NONE, NONE},
        .upperLimitChannel = {2047,2047,2047,2047,2047,2047,2047,2047},
        .lowerLimitChannel = {0,0,0,0,0,0,0,0}
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
        .rangeCheck = false,
        .rxNum = 0,
        .powerValue = POWER_VALUE_HIGH,
    };
    modelData = {
        .modelName = {0,0,0,0,0,0,0,0,0,0,0,0},
        .crc = CRC_VALUE
    };
}

void RadioData::storeData()
{
    int address = eepromSizeModel / MAX_NUMBER_OF_MODELS * selectedModel + eepromSizeGobal;

    // TODO REMOVE EEPROM SIZE PRINT
    /*
    Serial.printf("AnalogToDigitalData = %d\n",sizeof(AnalogToDigitalData));
    Serial.printf("ExpoData = %d\n",sizeof(ExpoData));
    Serial.printf("DualRateData = %d\n",sizeof(DualRateData));
    Serial.printf("TrimData = %d\n",sizeof(TrimData));
    Serial.printf("FunctionToChannelData = %d\n",sizeof(FunctionToChannelData));
    Serial.printf("MixerData = %d\n",sizeof(MixerData));
    Serial.printf("TransmitterData = %d\n",sizeof(TransmitterData));
    Serial.printf("ModelData = %d\n",sizeof(ModelData));
    Serial.printf("eepromSizeModel = %d\n",eepromSizeModel);
    Serial.printf("eepromSizeGobal = %d\n",eepromSizeGobal);
    */

    if(EEPROM.begin(eepromSizeModel + eepromSizeGobal) == true){
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

        EEPROM.put(address,mixerData);
        address += sizeof(MixerData);

        EEPROM.put(address,transmitterData);
        address += sizeof(TransmitterData);

        EEPROM.put(address,modelData);
        address += sizeof(ModelData);
    }
    EEPROM.end();
}

void RadioData::loadData()
{
    int address = eepromSizeModel / MAX_NUMBER_OF_MODELS * selectedModel + eepromSizeGobal;

    if(EEPROM.begin(eepromSizeModel + eepromSizeGobal) == true){
        EEPROM.get(address,analogToDigitalData);
        address += sizeof(AnalogToDigitalData);

        EEPROM.get(address,expoData);
        address += sizeof(ExpoData);

        EEPROM.get(address,dualRateData);
        address += sizeof(DualRateData);

        EEPROM.get(address,trimData);
        address += sizeof(TrimData);

        EEPROM.get(address,functionToChannelData);
        address += sizeof(FunctionToChannelData);

        EEPROM.get(address,mixerData);
        address += sizeof(MixerData);

        EEPROM.get(address,mixerData);
        address += sizeof(MixerData);

        EEPROM.get(address,transmitterData);
        address += sizeof(TransmitterData);

        EEPROM.get(address,modelData);
        address += sizeof(ModelData);
    }
    EEPROM.end();

    if(modelData.crc != CRC_VALUE){
        setDefaultValues();
    }
}

void RadioData::storeSelectedModel()
{
    if(EEPROM.begin(eepromSizeModel + eepromSizeGobal) == true){
        EEPROM.put(0, selectedModel);
    }
    EEPROM.end();
}

void RadioData::loadSelectedModel()
{
    if(EEPROM.begin(eepromSizeModel + eepromSizeGobal) == true){
        EEPROM.get(0, selectedModel);
    }
    EEPROM.end();
}

#endif