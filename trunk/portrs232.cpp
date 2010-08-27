#include "portrs232.h"

PortRs232::PortRs232(Configuration* config)
    : PortBase(config)
{
    isRunning = false;
    serialPort = 0;
}


void PortRs232::requestToStop(){
    isRunning = false;
}

PortRs232::~PortRs232(){

}

void PortRs232::portSetup(){
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
    serialPort->setBaudRate(baudRate);
    serialPort->setFlowControl(FLOW_OFF);
    serialPort->setParity(PAR_NONE);
    serialPort->setDataBits(DATA_8);
    serialPort->setStopBits(STOP_1);
}


void PortRs232::run(){
    if(isRunning) return;
    serialPort = new QextSerialPort(config->get("_setup_","port"),QextSerialPort::EventDriven);
    portSetup();

    if(serialPort->open(QIODevice::ReadWrite)){;
        portSetup();  //Note: on Windows settings must be called after serialPort->open, otherwise we get garbage when device is plugged for the fist time
        isRunning = true;
        serialPort->flush();
        while(isRunning && serialPort && serialPort->isOpen()){
            if(serialPort->bytesAvailable() > 0) emit newData(serialPort->readAll());
            qApp->processEvents();
        }
    }else{
        emit message("Could not open port "+config->get("_setup_","port")+"\nMake sure the port is available and not used by other applications.","critical");
    }

    isRunning = false;
    if(serialPort->isOpen()) serialPort->close();
    delete serialPort;
    emit stopped();
}



void PortRs232::send(const QString & str){
    QByteArray data;
    QRegExp rx("^(char|short|long|hex)\\:(.*)$");
    if(rx.exactMatch(str)){
        QString format = rx.capturedTexts()[1];
        QStringList items = rx.capturedTexts()[2].split(QRegExp("\\s*,\\s*"));
        qDebug(format.toAscii());
        for(int i=0;i<items.length();i++){
            qDebug(("["+items[i]+"]").toAscii());
            if("char"==format){
                short num = items[i].toShort();
                data.append((char)num);
            }else if("short"==format){
                short num = items[i].toShort();
                data.append((char*)(&num),sizeof(num));
            }else if("long"==format){
                long num = items[i].toLong();
                data.append((char*)(&num),sizeof(num));
            }else if("hex"==format){
                data.append(QByteArray::fromHex(items[i].toAscii()));
            }
        };
    }else{
        data.append(str);
    }
    qDebug(QString(data).toAscii());
    if(isRunning) serialPort->write(data);

}
