#include "c_mod_protocol.h"
#include "c_crc.h"

C_mod_protocol::C_mod_protocol(QObject *parent) : QObject(parent)
{

}

QByteArray C_mod_protocol::read_01020304PDU(MOD_FuncCode fcode, quint16 adr, quint16 sum)
{
    QByteArray array;
    def_uin16 temp;

    array.append(fcode);

    temp.value = adr;
    array.append(temp.by_1);
    array.append(temp.by_0);

    temp.value = sum;
    array.append(temp.by_1);
    array.append(temp.by_0);

    return array;
}

MB_ReplyBody C_mod_protocol::proc_01(quint8 byteSum, quint16 itemSum, QByteArray &array)
{
    // 计算 开关信号 bit位
    MB_ReplyBody res;
    quint8 T[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    for(int i=0;i<byteSum;i++)
    {
        quint8 by = array.at(i);
        for(int k=0;k<8;k++)   // 按比特位 "与" 运算
        {
            res.swt.append(T[k]&by);
        }
    }

    if(res.swt.size()>=itemSum)
    {
         res.swt = res.swt.mid(0,itemSum);
    }

    return res;
}

MB_ReplyBody C_mod_protocol::proc_02(quint8 byteSum, quint16 itemSum, QByteArray &array)
{
    // 计算 开关信号 bit位
    MB_ReplyBody res;
    quint8 T[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    for(int i=0;i<byteSum;i++)
    {
        quint8 by = array.at(i);
        for(int k=0;k<8;k++)   // 按比特位 "与" 运算
        {
            res.swt.append(T[k]&by);
        }
    }

    if(res.swt.size()>=itemSum)
    {
         res.swt = res.swt.mid(0,itemSum);
    }

    return res;
}

MB_ReplyBody C_mod_protocol::proc_03(quint16 itemSum, QByteArray &array)
{
    MB_ReplyBody res;
    for(int i=0;i<itemSum;i++)
    {
        def_uin16 temp;
        temp.by_1 = array.at(2*i);
        temp.by_0 = array.at(2*i+1);
        res.reg.append(temp.value);
    }

    return res;
}

MB_ReplyBody C_mod_protocol::proc_04(quint16 itemSum, QByteArray &array)
{
    MB_ReplyBody res;
    for(int i=0;i<itemSum;i++)
    {
        def_uin16 temp;
        temp.by_1 = array.at(2*i);
        temp.by_0 = array.at(2*i+1);
        res.reg.append(temp.value);
    }

    return res;
}
