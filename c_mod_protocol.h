#ifndef C_MOD_PROTOCOL_H
#define C_MOD_PROTOCOL_H

#include <QObject>
#include "mbtypedef.h"

/////////////////////
union def_uin16
{
    quint16 value;
    struct
    {
        quint8 by_0;
        quint8 by_1;
    };
};

//////////////////////////

class C_mod_protocol : public QObject
{
    Q_OBJECT

public:
    explicit C_mod_protocol(QObject *parent = 0);

signals:

public slots:

public:
    static QByteArray read_01020304PDU(enumMB_FuncCode fcode,quint16 adr,quint16 sum);
    static MB_ReplyBody proc_01(quint8 byteSum, quint16 itemSum, QByteArray &array);
    static MB_ReplyBody proc_02(quint8 byteSum, quint16 itemSum, QByteArray &array);
    static MB_ReplyBody proc_03(quint16 itemSum, QByteArray &array);
    static MB_ReplyBody proc_04(quint16 itemSum, QByteArray &array);
};

#endif // C_MOD_PROTOCOL_H
