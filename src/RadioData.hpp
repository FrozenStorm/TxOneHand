#ifndef RADIO_DATA_HPP
#define RADIO_DATA_HPP

#include <Preferences.h>
#include <list.h>
#include "Protocol.hpp"

#define MAX_NUMBER_OF_MODELS 16
#define MODEL_NAME_LENGTH 12
#define CHARACTER_SET_LENGTH 38
#define SUPPORTED_CHANNELS 8

class RadioData
{
private:
    char modelNameString[MODEL_NAME_LENGTH+1] = "123456789_12";
    Preferences pref;

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

    struct SensorToDigitalData{
        float seaLevelPressure;
    };
    SensorToDigitalData sensorToDigitalData;
    

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
        int throttle;
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

        float altitude = 0;
        float heading = 0;
        float pitch = 0;
        float roll = 0;
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
        volatile int channel[16] = {0};
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
    unsigned char selectedModel;
    struct ModelData
    {
        char modelName[MODEL_NAME_LENGTH];
    };
    ModelData modelData;

    void storeModelData();
    void loadModelData();
    void storeTrimData();
    void storeGlobalData();
    void loadGlobalData();    

    char* getModelName(void);
    
    RadioData(/* args */);
    ~RadioData();
};

RadioData::RadioData(/* args */)
{
    loadGlobalData();

    loadModelData();
    
analogToDigitalData = {
        .stickLimitUpDown = {.min = 0.16, .max = 2.82, .center = 1.34, .invert = true},
        .stickLimitLeftRight = {.min = 0.21, .max = 2.90, .center = 1.49, .invert = false},
        .stickLimitSlider = {.min = 0.0, .max = 1.32, .center = 0.65, .invert = true},
        .menuButtonLimit = {.up = 1.67, .down = 0.53, .left = 0.38, .right = 1.38, .center = 0.96},
        .menuButtonTolerance = 0.1,
        .longPressDurationMs = 600
    };
    sensorToDigitalData = {
        .seaLevelPressure = 1013.25
    };
    expoData = {
        .roll = 0.3,
        .pitch = 0.3,
        .throttle = 0
    };
    dualRateData = {
        .roll = 1,
        .pitch = 1,
        .throttle = 1
    };
    trimData = {
        .roll = 0,
        .pitch = 0,
        .throttle = 0
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
        .modelName = {2,13,22,13,2,0,0,0,0,0,0,0}
    };
}

RadioData::~RadioData()
{
}

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

void RadioData::storeGlobalData()
{
    pref.begin("Global");
    pref.putUChar("selectedModel", selectedModel);
    pref.end();
}

void RadioData::storeModelData()
{
    char name[15];

    sprintf(name,"Model-%d",selectedModel);
    pref.begin(name);

    pref.putFloat("atdd.slud.min", analogToDigitalData.stickLimitUpDown.min);
    pref.putFloat("atdd.slud.max", analogToDigitalData.stickLimitUpDown.max);
    pref.putFloat("atdd.slud.cen", analogToDigitalData.stickLimitUpDown.center);
    pref.putBool("atdd.slud.inv", analogToDigitalData.stickLimitUpDown.invert);

    pref.putFloat("atdd.sllr.min", analogToDigitalData.stickLimitUpDown.min);
    pref.putFloat("atdd.sllr.max", analogToDigitalData.stickLimitUpDown.max);
    pref.putFloat("atdd.sllr.cen", analogToDigitalData.stickLimitUpDown.center);
    pref.putBool("atdd.sllr.inv", analogToDigitalData.stickLimitUpDown.invert);

    pref.putFloat("atdd.sls.min", analogToDigitalData.stickLimitUpDown.min);
    pref.putFloat("atdd.sls.max", analogToDigitalData.stickLimitUpDown.max);
    pref.putFloat("atdd.sls.cen", analogToDigitalData.stickLimitUpDown.center);
    pref.putBool("atdd.sls.inv", analogToDigitalData.stickLimitUpDown.invert);

    pref.putFloat("atdd.mbl.up", analogToDigitalData.menuButtonLimit.up);
    pref.putFloat("atdd.mbl.dow", analogToDigitalData.menuButtonLimit.down);
    pref.putFloat("atdd.mbl.lef", analogToDigitalData.menuButtonLimit.left);
    pref.putFloat("atdd.mbl.rig", analogToDigitalData.menuButtonLimit.right);
    pref.putFloat("atdd.mbl.cen", analogToDigitalData.menuButtonLimit.center);

    pref.putFloat("atdd.mbt", analogToDigitalData.menuButtonTolerance);

    pref.putInt("atdd.lpdm", analogToDigitalData.longPressDurationMs);

    pref.putFloat("stdd.slp", sensorToDigitalData.seaLevelPressure);
    
    pref.putFloat("ed.rol", expoData.roll);
    pref.putFloat("ed.pit", expoData.pitch);
    pref.putFloat("ed.thr", expoData.throttle); 

    pref.putFloat("drd.rol", dualRateData.roll);
    pref.putFloat("drd.pit", dualRateData.pitch);
    pref.putFloat("drd.thr", dualRateData.throttle); 

    pref.putFloat("td.rol", trimData.roll);
    pref.putFloat("td.pit", trimData.pitch);
    pref.putFloat("td.thr", trimData.throttle); 

    pref.putFloat("md.ttp", mixerData.throttleToPitch);

    for(int i=0; i<SUPPORTED_CHANNELS; i++)
    {
        sprintf(name,"ftcd.ic.%d",i);
        pref.putBool(name, functionToChannelData.invertChannel[i]);
        sprintf(name,"ftcd.ftc.%d",i);
        pref.putInt(name, functionToChannelData.functionOnChannel[i]);
        sprintf(name,"ftcd.ulc.%d",i);
        pref.putInt(name, functionToChannelData.upperLimitChannel[i]);
        sprintf(name,"ftcd.llc.%d",i);
        pref.putInt(name, functionToChannelData.lowerLimitChannel[i]);
    }

    pref.putInt("td.bs", transmitterData.bindingState);
    pref.putInt("td.sp", transmitterData.selectedProtocol);
    pref.putInt("td.ssp", transmitterData.selectedSubProtocol);
    pref.putBool("td.rc", transmitterData.rangeCheck);
    pref.putInt("td.rn", transmitterData.rxNum);
    pref.putInt("td.bs", transmitterData.powerValue);

    pref.putBytes("md.mn", modelData.modelName, sizeof(modelData.modelName));
    
    pref.end();    
}

void RadioData::storeTrimData()
{
    char name[15];

    sprintf(name,"Model-%d",selectedModel);
    pref.begin(name);

    pref.putFloat("td.rol", trimData.roll);
    pref.putFloat("td.pit", trimData.pitch);
    pref.putFloat("td.thr", trimData.throttle); 
    
    pref.end();    
}

void RadioData::loadGlobalData()
{
    pref.begin("Global");
    selectedModel = pref.getUChar("sm", 0);
    pref.end();
}

void RadioData::loadModelData()
{
    char name[15];
    int readSize;

    sprintf(name,"Model-%d",selectedModel);
    pref.begin(name);

    analogToDigitalData.stickLimitUpDown.min = pref.getFloat("atdd.slud.min", 0.16);
    analogToDigitalData.stickLimitUpDown.max = pref.getFloat("atdd.slud.max", 2.82);
    analogToDigitalData.stickLimitUpDown.center = pref.getFloat("atdd.slud.cen", 1.34);
    analogToDigitalData.stickLimitUpDown.invert = pref.getBool("atdd.slud.inv", true);

    analogToDigitalData.stickLimitUpDown.min = pref.getFloat("atdd.sllr.min", 0.21);
    analogToDigitalData.stickLimitUpDown.max = pref.getFloat("atdd.sllr.max", 2.90);
    analogToDigitalData.stickLimitUpDown.center = pref.getFloat("atdd.sllr.cen", 1.49);
    analogToDigitalData.stickLimitUpDown.invert = pref.getBool("atdd.sllr.inv", false);

    analogToDigitalData.stickLimitUpDown.min = pref.getFloat("atdd.sls.min", 0.0);
    analogToDigitalData.stickLimitUpDown.max = pref.getFloat("atdd.sls.max", 1.32);
    analogToDigitalData.stickLimitUpDown.center = pref.getFloat("atdd.sls.cen", 0.65);
    analogToDigitalData.stickLimitUpDown.invert = pref.getBool("atdd.sls.inv", true);

    analogToDigitalData.menuButtonLimit.up = pref.getFloat("atdd.mbl.up", 1.67);
    analogToDigitalData.menuButtonLimit.down = pref.getFloat("atdd.mbl.dow", 0.53);
    analogToDigitalData.menuButtonLimit.left = pref.getFloat("atdd.mbl.lef", 0.38);
    analogToDigitalData.menuButtonLimit.right = pref.getFloat("atdd.mbl.rig", 1.38);
    analogToDigitalData.menuButtonLimit.center = pref.getFloat("atdd.mbl.cen", 0.96);

    analogToDigitalData.menuButtonTolerance = pref.getFloat("atdd.mbt", 0.1);

    analogToDigitalData.longPressDurationMs = pref.getInt("atdd.lpdm", 600);

    sensorToDigitalData.seaLevelPressure = pref.getFloat("stdd.slp", 1013.25);
    
    expoData.roll = pref.getFloat("ed.rol", 0.3);
    expoData.pitch = pref.getFloat("ed.pit", 0.3);
    expoData.throttle = pref.getFloat("ed.thr", 0); 

    dualRateData.roll = pref.getFloat("drd.rol", 1);
    dualRateData.pitch = pref.getFloat("drd.pit", 1);
    dualRateData.throttle = pref.getFloat("drd.thr", 1); 

    trimData.roll = pref.getFloat("td.rol", 0);
    trimData.pitch = pref.getFloat("td.pit", 0);
    trimData.throttle = pref.getFloat("td.thr", 0); 

    mixerData.throttleToPitch = pref.getFloat("md.ttp", 0);

    for(int i=0; i<SUPPORTED_CHANNELS; i++)
    {
        sprintf(name,"ftcd.ic.%d",i);
        functionToChannelData.invertChannel[i] = pref.getBool(name, false);
        sprintf(name,"ftcd.ftc.%d",i);
        functionToChannelData.functionOnChannel[i] = (Function)pref.getInt(name, NONE);
        sprintf(name,"ftcd.ulc.%d",i);
        functionToChannelData.upperLimitChannel[i] = pref.getInt(name, 2047);
        sprintf(name,"ftcd.llc.%d",i);
        functionToChannelData.lowerLimitChannel[i] = pref.getInt(name, 0);
    }

    transmitterData.bindingState = (BindingState)pref.getInt("td.bs", BINDED);
    transmitterData.selectedProtocol = pref.getInt("td.sp", 6);
    transmitterData.selectedSubProtocol = pref.getInt("td.ssp", 2);
    transmitterData.rangeCheck = pref.getBool("td.rc", false);
    transmitterData.rxNum = pref.getInt("td.rn", 0);
    transmitterData.powerValue = (PowerValue)pref.getInt("td.bs", POWER_VALUE_HIGH);
    

    readSize = pref.getBytes("md.mn", modelData.modelName, sizeof(modelData.modelName));
    if(readSize == 0) 
    {
        modelData = {
            .modelName = {2,13,22,13,2,0,0,0,0,0,0,0}
        };
    }
    
    pref.end();
}

#endif