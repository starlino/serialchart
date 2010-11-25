#include "decoderhdlc.h"

DecoderHdlc::DecoderHdlc(QObject *parent,Configuration *config) :
    DecoderBin(parent,config)
{
    hdlc_esc = QByteArray::fromHex(config->get("_setup_","hdlc_esc","7d").toAscii())[0];
    hdlc_sep = QByteArray::fromHex(config->get("_setup_","hdlc_sep","7e").toAscii())[0];
    hdlc_xor = QByteArray::fromHex(config->get("_setup_","hdlc_xor","20").toAscii())[0];
    escapeFlag = false;
}


void DecoderHdlc::newData(const QByteArray& newBytes){
    //qDebug(newBytes.toHex());
    for(int i=0;i<newBytes.length();i++){
        char c = newBytes[i];
        if(c == hdlc_sep){
            packetSeparator();
        }else{
            if(packetStarted){
                if(c == hdlc_esc &&  !escapeFlag){
                    escapeFlag = true;
                }else{
                    if(escapeFlag) c = c ^ hdlc_xor;
                    escapeFlag = false;
                    packetBytes.append(c);
                    if(packetBytes.length() > packetMaxLen){
                        packetBytes.clear();        //discard packet
                        packetStarted = false;
                    }
                }
            }
        }
    }
};


