#include "porthid.h"


PortHid::PortHid(QObject *parent,Configuration* config) :
    PortBase(parent,config)
{
    isRunning = false;
    hidDevice = 0;
}



void PortHid::start(){
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

            processDataLine();
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

void PortHid::processDataLine(){
    dataValues.clear();


    int j = 1; //start at byte 1 (byte 0 is Report ID)
    for(int i=0; i < config->fields.length(); i++){
        QString sectionName = config->fields[i];
        QString type = config->get(sectionName,"type","byte").toLower();
        int valueSize = 1;
        if("byte" == type){
            if(j+valueSize<=dataBytes.size())
                dataValues.append((double)(unsigned char)(dataBytes[j]));
        }else if("sbyte" == type){
            if(j+valueSize<=dataBytes.size())
                dataValues.append((double)(char)(dataBytes[j]));
        }else if("word" == type){
            valueSize = 2;
            if(j+valueSize<=dataBytes.size())
                dataValues.append(
                        (double)(unsigned char)(dataBytes[j])+
                        ((double)(unsigned char)(dataBytes[j+1]))*256
                );
        }else if("sword" == type){
            valueSize = 2;
            if(j+valueSize<=dataBytes.size())
                dataValues.append(
                       (double)(unsigned char)(dataBytes[j])+
                       ((double)(char)(dataBytes[j+1]))*256
                );
        }
        j += valueSize;
    }

    /*
    dataStr = "";
    for(int i=0;i< dataValues.size();i++){
        if(i>0) dataStr += ",";
        dataStr += QString::number(dataValues[i]);
    }
    */

    emit newDataLine(dataValues);
}

void PortHid::requestToStop(){
    isRunning = false;
}

PortHid::~PortHid(){

}
