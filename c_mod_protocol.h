#ifndef C_MOD_PROTOCOL_H
#define C_MOD_PROTOCOL_H

#include <QObject>

// 支持功能码
enum MOD_FuncCode
{
    func_01 =0x01,// 读(线圈)开出
    func_02 =0x02,// 读(离散输入)开入
    func_03 =0x03,//
    func_04 =0x04,//
    func_05 =0x05,//
    func_06 =0x06,//
    func_07 =0x07 //
};

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

struct MB_ReplyBody
{
    QByteArray swt;    // 开关量
    QList<quint16>reg; // 寄存器
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
    static QByteArray read_01020304PDU(MOD_FuncCode fcode,quint16 adr,quint16 sum);
    static MB_ReplyBody proc_01(quint8 byteSum, quint16 itemSum, QByteArray &array);
    static MB_ReplyBody proc_02(quint8 byteSum, quint16 itemSum, QByteArray &array);
    static MB_ReplyBody proc_03(quint16 itemSum, QByteArray &array);
    static MB_ReplyBody proc_04(quint16 itemSum, QByteArray &array);
};

#endif // C_MOD_PROTOCOL_H
