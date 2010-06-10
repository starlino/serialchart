#ifndef DECODERHDLC_H
#define DECODERHDLC_H

#include "decoderbase.h"

class DecoderHdlc : public DecoderBase
{
Q_OBJECT
public:
    explicit DecoderHdlc(QObject *parent = 0, Configuration* config = 0);

private:
    char hdlc_esc;
    char hdlc_sep;
    char hdlc_xor;
    bool escapeFlag;

signals:
    void newPacket(DecoderBase*);

public slots:
    void newData(const QByteArray&);
    void packetSeparator();
};

#endif // DECODERHDLC_H
