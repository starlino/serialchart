#ifndef PORTRS232_H
#define PORTRS232_H

#include "portbase.h"
#include "./qextserialport/qextserialport.h"

#define MAX_DATA_LINE_LEN   1024

class PortRs232 : public PortBase
{
Q_OBJECT
public:
    explicit PortRs232(QObject *parent = 0,Configuration* config = 0);
    ~PortRs232();
    void send(const QString & str);


private:
    QextSerialPort* serialPort;
    bool isRunning;

    void portSetup();
    void processDataLine();

signals:
    void newData(const QByteArray&);
    void packetSeparator();
    void stopped();

public slots:
    void start();
    void requestToStop();
};

#endif // PORTRS232_H
