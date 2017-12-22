#include "c_mod_protocol.h"
#include "c_crc.h"
#include "c_endian.h"

C_mod_protocol::C_mod_protocol(QObject *parent) : QObject(parent)
{

}

QByteArray C_mod_protocol::read_01020304PDU(enumMB_FuncCode fcode, quint16 adr, quint16 sum)
{
    QByteArray array;
    array.append(fcode);

    quint8 by0;
    quint8 by1;
    C_endian::uint16(ORDER_BIG,adr,by0,by1);
    array.append(by0);
    array.append(by1);

    C_endian::uint16(ORDER_BIG,sum,by0,by1);
    array.append(by0);
    array.append(by1);

    return array;
}

QByteArray C_mod_protocol::write_0X10(quint16 adr, quint16 sum, const QList<MBregister> &regList)
{
    QByteArray array;
    array.append(0X10);

    quint8 by0;
    quint8 by1;
    C_endian::uint16(ORDER_BIG,adr,by0,by1);
    array.append(by0);
    array.append(by1);

    C_endian::uint16(ORDER_BIG,sum,by0,by1);
    array.append(by0);
    array.append(by1);

    array.append(sum*2);

    foreach(MBregister reg,regList)
    {
        array.append(reg.byte_0);
        array.append(reg.byte_1);
    }

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
    MBregister rg;
    for(int i=0;i<itemSum;i++)
    {
        rg.byte_0 = array.at(2*i);
        rg.byte_1 = array.at(2*i+1);

        res.regList.append(rg);
    }

    return res;
}

MB_ReplyBody C_mod_protocol::proc_04(quint16 itemSum, QByteArray &array)
{
    MB_ReplyBody res;
    MBregister rg;
    for(int i=0;i<itemSum;i++)
    {
        rg.byte_0 = array.at(2*i);
        rg.byte_1 = array.at(2*i+1);

        res.regList.append(rg);
    }

    return res;
}

// 从机错误应答组包
QByteArray C_mod_protocol::Error_Reply(quint8 slaveAdr, enumMB_FuncCode fcode, RTU_Slave_ErrCode errCode)
{
    QByteArray res;
    res.append(slaveAdr);
    res.append(0X80+fcode);
    res.append(errCode);

    return res;
}
