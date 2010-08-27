#ifndef PORTRS232_H
#define PORTRS232_H

#include "portbase.h"
#include "./qextserialport/qextserialport.h"

#define MAX_DATA_LINE_LEN   1024

class PortRs232 : public PortBase
{
Q_OBJECT
public:
    explicit PortRs232(Configuration* config = 0);
    ~PortRs232();
    virtual void run();
    virtual void send(const QString & str);


private:
    QextSerialPort* serialPort;
    bool isRunning;

    void portSetup();
    void processDataLine();

signals:
    void newData(const QByteArray&);
    void packetSeparator();
    void stopped();
    void message(const QString& text,const QString& type);

public slots:
    void requestToStop();
};

#endif // PORTRS232_H
