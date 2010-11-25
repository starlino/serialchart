#ifndef DECODERBIN_H
#define DECODERBIN_H


#include "decoderbase.h"

class DecoderBin : public DecoderBase
{
Q_OBJECT
public:
    explicit DecoderBin(QObject *parent = 0, Configuration* config = 0);


signals:
    void newPacket(DecoderBase*);

public slots:
    void newData(const QByteArray&);
    void packetSeparator();
};


#endif // DECODERBIN_H
