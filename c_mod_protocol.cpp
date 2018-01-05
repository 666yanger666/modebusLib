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

MB_ReplyBody C_mod_protocol::proc_03_04(QByteArray array)
{
    MB_ReplyBody res;
    for(;;)
    {
        if(array.size()>=2)
        {
            MBregister rg;
            rg.byte_0 = array.at(0);
            rg.byte_1 = array.at(1);
            res.regList.append(rg);
            array.remove(0,2);
        }else
        {
            break;
        }
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

// bit开关量   给出bit量数目 计算需要的字节数
int C_mod_protocol::bitToByes(int bitSum)
{
    int res = bitSum/8;  //
    if(bitSum%8)
    {
        res+=1;   // 模不为零 +1
    }
    return res;
}

// bit开关位解析
MB_ReplyBody C_mod_protocol::proc_01_02(QByteArray array,quint16 paraSum)
{
    // 计算 开关信号 bit位
    MB_ReplyBody res;
    quint8 T[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    foreach(quint8 by,array)
    {
        for(int k=0;k<8;k++)   // 按比特位 "与" 运算
        {
            res.swtList.append(T[k]&by);  //非零:开  零:为关
        }
    }

    if(res.swtList.size()>paraSum)
    {
         res.swtList = res.swtList.mid(0,paraSum);
    }
    return res;
}
