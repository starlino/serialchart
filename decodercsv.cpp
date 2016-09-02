#include "decodercsv.h"

DecoderCsv::DecoderCsv(QObject *parent,Configuration* config) :
    DecoderBase(parent,config)
{

}


void DecoderCsv::newData(const QByteArray& newBytes){
    for(int i=0;i<newBytes.length();i++){
        char c = newBytes[i];
        if(13 == c || 10 == c){
            packetSeparator();
        }else{
            if(packetStarted){
                packetBytes.append(c);
                if(packetBytes.length() > packetMaxLen){
                    packetBytes.clear();        //discard packet
                    packetStarted = false;
                }
            }
        }
    }
};


void DecoderCsv::packetSeparator(){
    packetStarted = true;
    if(!packetBytes.isEmpty()){
        packetParts = packetBytes.split(',');
        packetValues.clear();
        QList<QByteArray>::iterator i;
        for(i=packetParts.begin();i!=packetParts.end();i++){
            packetValues.append((*i).toDouble());
        }
        emit newPacket(this);
    }
    packetBytes.clear();
};



