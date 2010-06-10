#include "portbase.h"
#include "portrs232.h"
#include "porthid.h"

PortBase::PortBase(QObject *parent,Configuration* config) :
    QObject(parent)
{
    this->config = config;
}


PortBase* createPort(QObject *parent, Configuration* config){
    if(0 == config->get("_setup_","port").compare("hid",Qt::CaseInsensitive))
        return new PortHid(parent,config);
    //default
        return new PortRs232(parent,config);
};


PortBase::~PortBase(){

}
