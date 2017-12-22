#include "c_mb_rtu_slave.h"
#include "c_crc.h"
#include "c_endian.h"

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
    this->m_sta = slave_CHECKREQ; // 进入检查请求
    this->m_recvBuf.append(array);  // 入缓存

    // 扫描帧
    for(;;)
    {
        //首字节 从机地址
        if(this->m_recvBuf.isEmpty())
        {
            break;
        }
        if((quint8)(this->m_recvBuf.at(0))!=this->m_slaveAdr)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }

        // 等功能码
        if(this->m_recvBuf.size()<2)
        {
            break;
        }

        // 功能码判断
        funcCodeTip funcTip;
        quint8 fcode = this->m_recvBuf.at(1);
        if(!funcTip.isContains(fcode))
        {
            // ..功能码不支持  返回错误码
            this->Error_Reply(this->m_slaveAdr,(enumMB_FuncCode)(this->m_recvBuf.at(1)),RTUslaveErr_REPLY01);
            this->m_recvBuf.remove(0,1);
            continue;
        }

        // 等待完整帧
        if(MB_func01==fcode || MB_func02==fcode || MB_func03==fcode || MB_func04==fcode)
        {
            if(this->m_recvBuf.size()<8)
            {
                break;
            }
        }else if(MB_func10==fcode)
        {
           // if(this)
        }

        //crc 校验
        quint16 crc = C_endian::uint16(ORDER_BIG,this->m_recvBuf.at(6),this->m_recvBuf.at(7));

        quint16 crcRes = C_CRC::crc16((quint8*)this->m_recvBuf.data(),6);
        if(crcRes!=crc) // crc 校验错误 不做任何返回
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

    quint16 adr = C_endian::uint16(ORDER_BIG,array.at(0),array.at(1));
    //...地址合法性检查

    if(adr>0x7D)
    {
         QByteArray fram;
         fram.append(this->m_slaveAdr);
         fram.append(0x81);
         fram.append(0x01);

         quint8 by0;
         quint8 by1;
         C_endian::uint16(ORDER_BIG,C_CRC::crc16((quint8*)fram.data(),3),by0,by1);
         fram.append(by0);
         fram.append(by1);

         // 发送回应

         return;
    }

    /*

    def_uin16 sum;
    sum.by_1 = array.at(0);
    sum.by_0 = array.at(1);
    //...地址 + 数量合法性检查
    */
}

void C_MB_RTU_SLAVE::proc_02(QByteArray &array)
{
    if(array.size()!=4)
    {
        return;
    }
/*
    def_uin16 adr;
    adr.by_1 = array.at(0);
    adr.by_0 = array.at(1);
    //...地址合法性检查

    def_uin16 sum;
    sum.by_1 = array.at(0);
    sum.by_0 = array.at(1);
    //...地址 + 数量合法性检查
    */
}

void C_MB_RTU_SLAVE::proc_03(QByteArray &array)
{
    if(array.size()!=4)
    {
        return;
    }
/*
    def_uin16 adr;
    adr.by_1 = array.at(0);
    adr.by_0 = array.at(1);
    //...地址合法性检查

    def_uin16 sum;
    sum.by_1 = array.at(0);
    sum.by_0 = array.at(1);
    //...地址 + 数量合法性检查
    */
}

void C_MB_RTU_SLAVE::proc_04(QByteArray &array)
{
    if(array.size()!=4)
    {
        return;
    }
/*
    def_uin16 adr;
    adr.by_1 = array.at(0);
    adr.by_0 = array.at(1);
    //...地址合法性检查

    def_uin16 sum;
    sum.by_1 = array.at(0);
    sum.by_0 = array.at(1);
    //...地址 + 数量合法性检查
    */
}

void C_MB_RTU_SLAVE::Error_Reply(quint8 slaveAdr, enumMB_FuncCode fcode, RTU_Slave_ErrCode errCode)
{
    QByteArray array= C_mod_protocol::Error_Reply(slaveAdr,fcode,errCode);

    quint16 crc = C_CRC::crc16((quint8*)array.data(),array.size());

    quint8 by0;
    quint8 by1;
    C_endian::uint16(ORDER_BIG,crc,by0,by1);
    array.append(by0);
    array.append(by1);

    this->sendData(array);
}

void C_MB_RTU_SLAVE::sendData(QByteArray &array)
{
    emit sig_sendData(array);
}
