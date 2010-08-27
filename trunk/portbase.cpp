#include "portbase.h"
#include "portrs232.h"
#include "porthid.h"

PortBase::PortBase(Configuration* config) :
    QThread()
{
    this->config = config;
}


PortBase* createPort(Configuration* config){
    if(0 == config->get("_setup_","port").compare("hid",Qt::CaseInsensitive))
        return new PortHid(config);
    //default
        return new PortRs232(config);
};


PortBase::~PortBase(){

}
