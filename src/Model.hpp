#ifndef MODEL_HPP
#define MODEL_HPP

#include "RadioClass.hpp"

class Model : public RadioClass
{
private:
    enum MenuEntries{SELECTED_MODEL, MODEL_NAME, NUMBER_OF_MENUENTRIES};
    MenuEntries selectedMenuEntry = NUMBER_OF_MENUENTRIES;

    unsigned int modelNameInputPosition = 0;
    unsigned int modelNameInputCharacter = 0;

public:
    Model(TFT_eSPI& newTft, RadioData& newRadioData) : RadioClass(newTft, newRadioData){}
    void doFunction();

    void showMenu();
    void showValue();
    const char * getTitle();
    void up();
    void down();
    bool left();
    bool right();
    void center();
};


void Model::showValue(){};
const char * Model::getTitle()
{
    return "Model";
}

void Model::showMenu()
{
    sprintf(myString,"Model = %-2d\n",radioData.selectedModel);
    tft.drawString(myString, posW, posH+incH*0);

    sprintf(myString,"Name = % -25s  \n",radioData.getModelName()); // TODO länge abhängig machen von define
    tft.drawString(myString, posW, posH+incH*1);

    drawMenuPointer(selectedMenuEntry,NUMBER_OF_MENUENTRIES);
}

void Model::up()
{
    if(selectedMenuEntry > 0) selectedMenuEntry=(MenuEntries)(selectedMenuEntry-1);
    else selectedMenuEntry = NUMBER_OF_MENUENTRIES;
}
void Model::down()
{
    if(selectedMenuEntry < NUMBER_OF_MENUENTRIES) selectedMenuEntry=(MenuEntries)(selectedMenuEntry+1);
    else selectedMenuEntry = (MenuEntries)0;
}
bool Model::left()
{
    switch (selectedMenuEntry)
    {
    case SELECTED_MODEL:
        radioData.storeData();
        if(radioData.selectedModel > 0) radioData.selectedModel--;
        else radioData.selectedModel = MAX_NUMBER_OF_MODELS-1;
        radioData.loadData();
        radioData.storeSelectedModel();
        modelNameInputPosition = 0;
        modelNameInputCharacter = radioData.modelData.modelName[modelNameInputPosition];
        break;
    case MODEL_NAME:
        if(modelNameInputCharacter > 0) modelNameInputCharacter--;
        else modelNameInputCharacter = CHARACTER_SET_LENGTH-1;
        radioData.modelData.modelName[modelNameInputPosition] = modelNameInputCharacter;
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}
bool Model::right()
{
    switch (selectedMenuEntry)
    {
    case SELECTED_MODEL:
        radioData.storeData();
        if(radioData.selectedModel < MAX_NUMBER_OF_MODELS-1) radioData.selectedModel++;
        else radioData.selectedModel = 0;
        radioData.loadData();
        radioData.storeSelectedModel();
        modelNameInputPosition = 0;
        modelNameInputCharacter = radioData.modelData.modelName[modelNameInputPosition];
        break;
    case MODEL_NAME:
        if(modelNameInputCharacter < CHARACTER_SET_LENGTH-1) modelNameInputCharacter++;
        else modelNameInputCharacter = 0;
        radioData.modelData.modelName[modelNameInputPosition] = modelNameInputCharacter;
        break;
    case NUMBER_OF_MENUENTRIES:
        return true;
        break;
    default:
        break;
    }
    return false;
}

void Model::center()
{
    switch (selectedMenuEntry)
    {
    case SELECTED_MODEL:
        break;
    case MODEL_NAME:
        radioData.modelData.modelName[modelNameInputPosition] = modelNameInputCharacter;
        if(modelNameInputPosition < MODEL_NAME_LENGTH-1) modelNameInputPosition++;
        else modelNameInputPosition = 0;
        modelNameInputCharacter = radioData.modelData.modelName[modelNameInputPosition];
        break;
    case NUMBER_OF_MENUENTRIES:
        break;
    default:
        break;
    }
}

void Model::doFunction()
{

}

#endif