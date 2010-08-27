#ifndef PORTHID_H
#define PORTHID_H

#include "portbase.h"
#include "hiddevice.h"

class PortHid : public PortBase
{
Q_OBJECT
public:
    explicit PortHid(Configuration* config = 0);
    ~PortHid();
    virtual void run();
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
    void message(const QString& text,const QString& type);

    void newDataLine(const QList<double>&);


public slots:
    //void start();
    void requestToStop();
};

#endif // PORTHID_H
