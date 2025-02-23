#ifndef MODEL_HPP
#define MODEL_HPP

#include "RadioClass.hpp"

class Model : public RadioClass
{
private:

public:
    Model(RadioData& newRadioData) : RadioClass(newRadioData){}
    void doFunction();
};

void Model::doFunction()
{

}

#endif