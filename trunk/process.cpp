#include "process.h"

Process::Process(QObject *parent,Configuration* config) :
    QObject(parent)
{
    this->config = config;
    isRunning = false;
    port = 0;
}

void Process::portSetup(){
    BaudRateType baudRate = BAUD9600;
    switch(config->get("_setup_","baudrate").toInt()){
        case 110: baudRate = BAUD110; break;
        case 300: baudRate = BAUD300; break;
        case 600: baudRate = BAUD600; break;
        case 1200: baudRate = BAUD1200; break;
        case 2400: baudRate = BAUD2400; break;
        case 9600: baudRate = BAUD9600; break;
        case 19200: baudRate = BAUD19200; break;
        case 38400: baudRate = BAUD38400; break;
        case 57600: baudRate = BAUD57600; break;
        case 115200: baudRate = BAUD115200; break;
    }
    port->setBaudRate(baudRate);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
}


void Process::start(){
    if(isRunning) return;
    port = new QextSerialPort(config->get("_setup_","port"),QextSerialPort::EventDriven);
    portSetup();

    if(port->open(QIODevice::ReadOnly)){;
        portSetup();  //Note: on Windows settings must be called after port->open, otherwise we get garbage when device is plugged for the fist time

        newLine = true;
        isRunning = true;

        port->flush();
        dataLine = "test";

        while(isRunning && port && port->isOpen()){
            int bytesAvailable = port->bytesAvailable();
            if(bytesAvailable > 0){
                if(newLine){
                    dataLine = "";
                    dataLineInvalid = 0;
                    newLine = false;
                }
                static char c;
                port->read((char*)(&c),1);
                if(13 == c || 10 == c){
                    if(!dataLine.isEmpty() && !dataLineInvalid) processDataLine(); //ignore empty and invalid lines
                    newLine = true;
                }else{
                    if(dataLine.length()<MAX_DATA_LINE_LEN)
                        dataLine.append(c);
                    else
                        dataLineInvalid = 1;

                }
            }
            qApp->processEvents();
        }
    }else{
        QMessageBox::critical(0,"","Could open port "+config->get("_setup_","port")+"\nMake sure the port is available and not used by other applications.");
    }

    isRunning = false;

    if(port->isOpen()) port->close();
    delete port;

    emit stopped();

}

void Process::processDataLine(){
    QStringList itemList = dataLine.split(",");
    data.clear();
    QStringList::iterator i;
    for(i=itemList.begin();i!=itemList.end();i++){
        data.append((*i).toDouble());
    }
    emit newDataLine(data);
}

void Process::requestToStop(){
    isRunning = false;
}

Process::~Process(){

}
