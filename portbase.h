#ifndef PORTBASE_H
#define PORTBASE_H

#include <QObject>
#include "configuration.h"
#include "decoderbase.h"

class PortBase : public QObject
{
Q_OBJECT
public:
    explicit PortBase(QObject *parent = 0,Configuration* config = 0);
    virtual ~PortBase();
    DecoderBase* decoder;
    virtual void send(const QString & str) = 0;


protected:
    Configuration* config;
    QByteArray dataBytes;
    QList<double> dataValues;


signals:
    //void stopped() = 0;
    //void newData(const QByteArray&);

public slots:
    virtual void start() = 0;
    virtual void requestToStop() = 0;


};

PortBase* createPort(QObject *parent, Configuration* config);

#endif // PORTBASE_H
