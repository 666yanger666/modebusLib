#include "c_mb_rtu_master.h"
#include "c_crc.h"
#include <QDebug>
#include <QMessageBox>

C_MB_RTU_MASTER::C_MB_RTU_MASTER(QObject *parent) : QObject(parent)
{
     connect(&this->m_replytimer,&QTimer::timeout,this,C_MB_RTU_MASTER::slot_replyTimer);
     this->m_timeout = 3000; //响应超时
     this->m_sta = RTUmaster_IDEL;
}

void C_MB_RTU_MASTER::sendData(QByteArray &array)
{
    emit this->sig_sendData(array);
}

//  响应超时
void C_MB_RTU_MASTER::slot_replyTimer()
{
    //this->m_sta = master_ERRPROC;
    //..

    this->m_sta = RTUmaster_IDEL;
    this->m_replytimer.stop();

    qDebug()<<"超时！";
}

void C_MB_RTU_MASTER::slot_recvData(QByteArray &array)
{
    //非等待应答  不接收数据
    if(this->m_sta!=RTUmaster_WAITREPLY)
    {
        return;
    }
    this->m_recvBuf.append(array);

    // 扫描帧
    for(;;)
    {
        //首字节 从机地址
        if(this->m_recvBuf.isEmpty())
        {
            break;
        }
        if(this->m_recvBuf.at(0)!=this->m_queryTrans.trans.slaveAdr)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }

        //功能码(是否异常码)
        if(this->m_recvBuf.size()<2)
        {
            break;
        }
        if(quint8(this->m_recvBuf.at(1)&0X80) == 0X80)
        {
            if(this->m_recvBuf.size()<5)
            {
                break;
            }

            // crc 校验检查
            def_uin16  crc;
            crc.by_1 = this->m_recvBuf.at(3);
            crc.by_0 = this->m_recvBuf.at(4);

            if(C_CRC::crc16((quint8*)this->m_recvBuf.data(),3)!=crc.value)
            {
                this->m_recvBuf.remove(0,1);  // 校验错
                continue;
            }

            // 功能码检查
            if((quint8)((this->m_recvBuf.at(1) -0x80)) !=(quint8)this->m_queryTrans.trans.funcCode)
            {
                this->m_recvBuf.remove(0,1);  // 功能码错
                continue;
            }

            // 解析异常码
            quint8 err = quint8(this->m_recvBuf.at(2));

            RTU_Master_ErrCode errCode;
            if(err>=0x01 && err <=0x04)
            {
                errCode = RTU_Master_ErrCode(err);
            }else
            {
                errCode = RTUmaster_UNKOWN;
            }
            qDebug()<<"异常码:"<<this->m_recvBuf.at(2)<<errCode;
            this->m_sta = RTUmaster_IDEL; // 空闲状态
            this->m_replytimer.stop();
            break;
        }

        // 检查功能码
        if(this->m_recvBuf.at(1)!=this->m_queryTrans.trans.funcCode)
        {
            this->m_recvBuf.remove(0,1);  // 功能码错
            continue;
        }

        if(this->m_recvBuf.size()<5+this->m_byteSum)
        {
            break;  // 等待继续接收
        }

        // crc 校验
        def_uin16  crc;
        crc.by_1 = this->m_recvBuf.at(3+this->m_byteSum);
        crc.by_0 = this->m_recvBuf.at(4+this->m_byteSum);
        qDebug()<<C_CRC::crc16((quint8*)this->m_recvBuf.data(),3+this->m_byteSum);
        if(C_CRC::crc16((quint8*)this->m_recvBuf.data(),3+this->m_byteSum)!=crc.value)
        {
            this->m_recvBuf.remove(0,1);  // 校验错
            continue;
        }

        QByteArray tArray;
        tArray =  this->m_recvBuf.mid(3,this->m_byteSum);  // 截取数据
        this->m_recvBuf.remove(0,5+this->m_byteSum);  //  删除数据帧

        //处理应答
        this->m_sta = RTUmaster_PROCREPLY;
        if(0x01==this->m_queryTrans.trans.funcCode)
        {
            this->proc_01(tArray);
        }else if(0x02==this->m_queryTrans.trans.funcCode)
        {
            this->proc_02(tArray);
        }else if(0x03==this->m_queryTrans.trans.funcCode)
        {
            this->proc_03(tArray);
        }else if(0x04==this->m_queryTrans.trans.funcCode)
        {
            this->proc_04(tArray);
        }

        this->m_sta = RTUmaster_IDEL; // 空闲状态
        this->m_replytimer.stop();
    }
}

// 解析 01 pdu
void C_MB_RTU_MASTER::proc_01(QByteArray &array)
{
    if(array.size() != this->m_byteSum)
    {
        return;
    }
    if(this->m_byteSum!=(this->m_queryTrans.trans.paraSum+8)/8)
    {
        return;
    }
    MB_ReplyBody res = C_mod_protocol::proc_01(this->m_byteSum,this->m_queryTrans.trans.paraSum,array);

    // 发送解析结果信号
    emit sig_proc(this->m_queryTrans.transID,this->m_queryTrans.trans.slaveAdr,this->m_queryTrans.trans.funcCode,res);
}

void C_MB_RTU_MASTER::proc_02(QByteArray &array)
{
    if(array.size() != this->m_byteSum)
    {
        return;
    }
    if(this->m_byteSum!=(this->m_queryTrans.trans.paraSum+8)/8)
    {
        return;
    }

    MB_ReplyBody res = C_mod_protocol::proc_02(this->m_byteSum,this->m_queryTrans.trans.paraSum,array);

    emit sig_proc(this->m_queryTrans.transID,this->m_queryTrans.trans.slaveAdr,this->m_queryTrans.trans.funcCode,res);
}

// 解析03 PDU 数据
void C_MB_RTU_MASTER::proc_03(QByteArray &array)
{ 
    if(array.size() != this->m_byteSum)
    {
        return;
    }
    if(this->m_byteSum!=this->m_queryTrans.trans.paraSum*2)
    {
        return;
    }

    MB_ReplyBody res = C_mod_protocol::proc_03(this->m_queryTrans.trans.paraSum,array);
    emit sig_proc(this->m_queryTrans.transID,this->m_queryTrans.trans.slaveAdr,this->m_queryTrans.trans.funcCode,res);
}

// 解析03 PDU 数据
void C_MB_RTU_MASTER::proc_04(QByteArray &array)
{
    if(array.size() != this->m_byteSum)
    {
        return;
    }
    if(this->m_byteSum!=this->m_queryTrans.trans.paraSum*2)
    {
        return;
    }

    MB_ReplyBody res = C_mod_protocol::proc_04(this->m_queryTrans.trans.paraSum,array);
    emit sig_proc(this->m_queryTrans.transID,this->m_queryTrans.trans.slaveAdr,this->m_queryTrans.trans.funcCode,res);
}

void C_MB_RTU_MASTER::setTimeOut(int ms)
{
    this->m_timeout = ms;
}

void C_MB_RTU_MASTER::queryCMD(MBRequestTransEx trans)
{
    if(trans.trans.funcCode!=MB_func01 &&
       trans.trans.funcCode!=MB_func02 &&
       trans.trans.funcCode!=MB_func03 &&
       trans.trans.funcCode!=MB_func04)
    {
        return;
    }

    // 组帧
    QByteArray array;

    array.append(trans.trans.slaveAdr);
    array.append(C_mod_protocol::read_01020304PDU(trans.trans.funcCode,trans.trans.beginAdr,trans.trans.paraSum));

    quint16 crc = C_CRC::crc16((quint8*)array.data(),array.size());

    def_uin16 temp;
    temp.value = crc;
    array.append(temp.by_1);
    array.append(temp.by_0);

    // 清空接收缓存发送数据
    this->m_recvBuf.clear();

    // 计算正常应答 数据字节数
    if(MB_func01==trans.trans.funcCode)
    {
        this->m_byteSum = (trans.trans.paraSum+8)/8;
    }else if(MB_func02==trans.trans.funcCode)
    {
        this->m_byteSum = (trans.trans.paraSum+8)/8;
    }else if(MB_func03==trans.trans.funcCode)
    {
        this->m_byteSum = trans.trans.paraSum*2;
    }else if(MB_func04==trans.trans.funcCode)
    {
        this->m_byteSum = trans.trans.paraSum*2;
    }

    // 等待应答，启动超时定时
    this->m_sta = RTUmaster_WAITREPLY;  // 进入：--等待应答--
    this->m_queryTrans = trans;

    // 发送请求
    this->sendData(array);

    // 启动应答超时定时器
    this->m_replytimer.start(this->m_timeout);
}

// 协议驱动状态机是否空闲
bool C_MB_RTU_MASTER::isIdle()
{
    if(RTUmaster_IDEL ==this->m_sta)
    {
        return true;
    }
    return false;
}

