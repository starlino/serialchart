#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include "configuration.h"
#include "./qextserialport/qextserialport.h"

#define MAX_DATA_LINE_LEN   1024

class Process : public QObject
{
Q_OBJECT
public:
    explicit Process(QObject *parent = 0,Configuration* config = 0);
    ~Process();
    QString dataLine;
    int dataLineInvalid;
    QList<double> data;

private:
    Configuration* config;
    QextSerialPort* port;
    bool isRunning;
    bool newLine;
    void portSetup();
    void processDataLine();

signals:
    void newDataLine(const QList<double>&);
    void stopped();

public slots:
    void start();
    void requestToStop();


};

#endif // PROCESS_H
