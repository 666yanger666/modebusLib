#ifndef C_MOD_PROTOCOL_H
#define C_MOD_PROTOCOL_H

#include <QObject>
#include "../../../WORK/HLMonitorGit/include/include.h"

class C_mod_protocol : public QObject
{
    Q_OBJECT

public:
    explicit C_mod_protocol(QObject *parent = 0);

signals:

public slots:

public:
    static QByteArray read_01020304PDU(enumMB_FuncCode fcode,quint16 adr,quint16 sum);
    static QByteArray write_0X10(quint16 adr,quint16 sum,const QList<MBregister>&regList);

    static MB_ReplyBody proc_01_02(QByteArray array,quint16 paraSum);
    static MB_ReplyBody proc_03_04(QByteArray array);
    static QByteArray Error_Reply(quint8 slaveAdr,enumMB_FuncCode fcode,RTU_Slave_ErrCode errCode);

    static int bitToByes(int bitSum);
};

#endif // C_MOD_PROTOCOL_H
