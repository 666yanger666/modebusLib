#include "c_mb_rtu_slave.h"
#include "c_crc.h"

C_MB_RTU_SLAVE::C_MB_RTU_SLAVE(QObject *parent) : QObject(parent)
{
    this->m_sta = slave_IDEL;

    for(int i=0;i<125;i++)
    {
        this->m_regDataCFG.append(false);
    }
}

void C_MB_RTU_SLAVE::slot_recvData(QByteArray &array)
{
    this->m_recvBuf.append(array);  // 入缓存

    // 扫描帧
    for(;;)
    {
        this->m_sta = slave_CHECKREQ; // 进入检查请求
        //首字节 从机地址
        if(this->m_recvBuf.isEmpty())
        {
            break;
        }
        if(this->m_recvBuf.at(0)!=this->m_slaveAdr)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }

        // 等功能码
        if(this->m_recvBuf.size()<2)
        {
            break;
        }

        // 01 02 03 04 功能请求命令 帧长 8个字节
        QList<quint8>flist;
        flist<<0x01<<0x02<<0x03<<0x04;
        if(!flist.contains(this->m_recvBuf.at(1)))
        {
            //... 返回 异常码 01  不支持的功能码
        }

        if(this->m_recvBuf.size()<8) // 等待完整帧
        {
            break;
        }

        //crc 校验
        def_uin16 crc;
        crc.by_1 = this->m_recvBuf.at(6);
        crc.by_0 = this->m_recvBuf.at(7);

        quint16 crcRes = C_CRC::crc16((quint8*)this->m_recvBuf.data(),6);
        if(crcRes!=crc.value) // crc 校验错误 不做任何返回
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }
        this->m_sta =  slave_PROCREQ;// 处理请求动作状态
        // dispatch
        QByteArray tFram = this->m_recvBuf.mid(0,8);
        this->m_recvBuf.remove(0,8);
        quint8 funcID = tFram.at(1);
        QByteArray tPara = tFram.mid(2,4);
        if(0x01 == funcID)
        {
            this->proc_01(tPara);
        }else if(0x02 == funcID)
        {
            this->proc_02(tPara);
        }else if(0x03 == funcID)
        {
            this->proc_03(tPara);
        }else if(0x04 == funcID)
        {
            this->proc_04(tPara);
        }
        this->m_sta = slave_IDEL; // 空闲状态
    }
}

void C_MB_RTU_SLAVE::proc_01(QByteArray &array)
{
    if(array.size()!=4)
    {
        return;
    }

    def_uin16 adr;
    adr.by_1 = array.at(0);
    adr.by_0 = array.at(1);
    //...地址合法性检查

    if(adr.value>0x7D)
    {
         QByteArray fram;
         fram.append(this->m_slaveAdr);
         fram.append(0x81);
         fram.append(0x01);

         def_uin16 crc;
         crc.value = C_CRC::crc16((quint8*)fram.data(),3);
         fram.append(crc.by_1);
         fram.append(crc.by_0);

         // 发送回应

         return;
    }

    //if()

    def_uin16 sum;
    sum.by_1 = array.at(0);
    sum.by_0 = array.at(1);
    //...地址 + 数量合法性检查
}

void C_MB_RTU_SLAVE::proc_02(QByteArray &array)
{
    if(array.size()!=4)
    {
        return;
    }

    def_uin16 adr;
    adr.by_1 = array.at(0);
    adr.by_0 = array.at(1);
    //...地址合法性检查

    def_uin16 sum;
    sum.by_1 = array.at(0);
    sum.by_0 = array.at(1);
    //...地址 + 数量合法性检查
}

void C_MB_RTU_SLAVE::proc_03(QByteArray &array)
{
    if(array.size()!=4)
    {
        return;
    }

    def_uin16 adr;
    adr.by_1 = array.at(0);
    adr.by_0 = array.at(1);
    //...地址合法性检查

    def_uin16 sum;
    sum.by_1 = array.at(0);
    sum.by_0 = array.at(1);
    //...地址 + 数量合法性检查
}

void C_MB_RTU_SLAVE::proc_04(QByteArray &array)
{
    if(array.size()!=4)
    {
        return;
    }

    def_uin16 adr;
    adr.by_1 = array.at(0);
    adr.by_0 = array.at(1);
    //...地址合法性检查

    def_uin16 sum;
    sum.by_1 = array.at(0);
    sum.by_0 = array.at(1);
    //...地址 + 数量合法性检查
}
