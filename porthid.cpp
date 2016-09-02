#include "porthid.h"

PortHid::PortHid(Configuration* config)
    : PortBase(config)
{
    isRunning = false;
    hidDevice = 0;
}



void PortHid::run(){
    if(isRunning) return;
    hidDevice = new HidDevice();
    if(hidDevice->connect("vid_"+config->get("_setup_","vid")+"&"+"pid_"+config->get("_setup_","pid"))){
        isRunning = true;
        unsigned char in_report_id = config->get("_setup_","in_report_id","0").toInt();
        int in_endpoint_size = config->get("_setup_","in_endpoint_size","64").toInt();


        dataBytes.resize(in_endpoint_size +1);

        while(isRunning){
            //read
            hidDevice->read(in_report_id,dataBytes);
            emit newData(dataBytes);
            emit packetSeparator();
            qApp->processEvents();
        }

    }else{
        QMessageBox::critical(0,"","Could not open USB HID with VID="+config->get("_setup_","vid")+" PID="+config->get("_setup_","pid")+"\nMake sure the device is attached.");

    };

    isRunning = false;

    delete hidDevice;
    emit stopped();

}

void PortHid::send(const QString & str){

}


void PortHid::requestToStop(){
    isRunning = false;
}

PortHid::~PortHid(){

}
