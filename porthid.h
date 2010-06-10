#ifndef PORTHID_H
#define PORTHID_H

#include "portbase.h"
#include "hiddevice.h"

class PortHid : public PortBase
{
Q_OBJECT
public:
    explicit PortHid(QObject *parent = 0,Configuration* config = 0);
    ~PortHid();
    virtual void send(const QString & str);






private:
    HidDevice* hidDevice;
    bool isRunning;
    bool newLine;
    void portSetup();
    void processDataLine();

signals:
    void newData(const QByteArray&);
    void stopped();

    void newDataLine(const QList<double>&);


public slots:
    void start();
    void requestToStop();
};

#endif // PORTHID_H
