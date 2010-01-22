#include "configuration.h"

Configuration::Configuration(QObject *parent) :
    QObject(parent)
{

}

const QString Configuration::get(const QString& sectionName,const QString& propertyName,const QString& defaultValue){

    if(sections.contains(sectionName) && sections[sectionName].contains(propertyName)){
        return sections[sectionName][propertyName];
    }else{
        if("" == defaultValue && sectionName!="_setup_" && sectionName!="_default_")
            return get("_default_",propertyName);
        else
            return defaultValue;
    }
};


void Configuration::parse(const QString& str){
    //load defaults
    columns.clear();
    sections.clear();

    sections["_setup_"]["port"] = "COM1";
    sections["_setup_"]["baudrate"] = "9600";
    sections["_setup_"]["min"] = "-100";
    sections["_setup_"]["max"] = "100";

    sections["_default_"] ["color"] = "black";   //http://www.w3.org/TR/SVG/types.html#ColorKeywords
    sections["_default_"] ["min"] = "-100";
    sections["_default_"] ["max"] = "100";


    //parse configuration
    QString sectionName = "_setup_";
    QStringList lines = str.split(QRegExp("[\\x0D\\x0A]+"));
    QStringList::iterator i;
    for(i=lines.begin();i!=lines.end();i++){
        QString line = *i;
        //qDebug(("{"+ line +"}").toAscii());
        QRegExp rx("^\\s*\\[\\s*(\\w+)\\s*\\]\\s*$");
        if(rx.exactMatch(line)){
            //qDebug((" section["+rx.capturedTexts()[1]+"]").toAscii());
            sectionName = rx.capturedTexts()[1];
            if(!columns.contains(sectionName) && sectionName!="_setup_" && sectionName!="_default_") columns.append(sectionName);;
            continue;
        }
        rx.setPattern("^\\s*(\\w+)\\s*\\=\\s*(\\S+)\\s*");
        if(rx.exactMatch(line)){
            sections[sectionName][rx.capturedTexts()[1]] = rx.capturedTexts()[2];
           // qDebug((" pair["+rx.capturedTexts()[1]+","+rx.capturedTexts()[2]+"]").toAscii());
            continue;
        }
    }

}



