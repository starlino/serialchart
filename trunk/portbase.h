#ifndef PORTBASE_H
#define PORTBASE_H

#include <QObject>
#include "configuration.h"
#include "decoderbase.h"

class PortBase : public QThread
{
Q_OBJECT
public:
    explicit PortBase(Configuration* config = 0);
    virtual ~PortBase();
    DecoderBase* decoder;

    virtual void run() = 0;
    virtual void send(const QString & str) = 0;


protected:
    Configuration* config;
    QByteArray dataBytes;
    QList<double> dataValues;


signals:
    //void stopped() = 0;
    //void newData(const QByteArray&);
    //void message(const QString& text,const QString& type);

public slots:
    //virtual void start() = 0;
    virtual void requestToStop() = 0;


};

PortBase* createPort(Configuration* config);

#endif // PORTBASE_H
