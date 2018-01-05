#include "c_mb_rtu_master.h"
#include "c_crc.h"
#include <QDebug>
#include <QMessageBox>
#include "c_endian.h"
#include "../../../WORK/HLMonitorGit/include/include.h"

C_MB_RTU_MASTER::C_MB_RTU_MASTER(QObject *parent) : QObject(parent)
{
     connect(&this->m_replytimer,&QTimer::timeout,this,&C_MB_RTU_MASTER::slot_replyTimer);
     this->m_sta = RTUmaster_IDEL;

     // 启动应答超时定时器
     this->m_replytimer.start(1000);  //
}

void C_MB_RTU_MASTER::sendData(QByteArray &array)
{
    emit this->sig_sendData(array);
}

//  响应超时
void C_MB_RTU_MASTER::slot_replyTimer()
{  
    if(this->m_sta!=RTUmaster_IDEL)
    {
        this->m_timerSUM+=1;
        if(this->m_timerSUM>3)
        {
            this->m_recvBuf.clear();
            this->m_sta = RTUmaster_IDEL;
            qDebug()<<"::超时！";
            emit sig_Error(this->m_queryTrans.transID,this->m_queryTrans.trans.slaveAdr,this->m_queryTrans.trans.funcCode,RTUmasterErr_TIMEOUT);
        }
    }
}

void C_MB_RTU_MASTER::slot_recvData(QByteArray &array)
{
    QString strT;
    foreach(quint8 ch,array)
    {
        strT+=QString::number(ch,16);
        strT+=" ";
    }
    qDebug()<<"RECV:"<<strT;

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
        if((quint8)(this->m_recvBuf.at(0))!=this->m_queryTrans.trans.slaveAdr)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }

        //功能码(是否异常码)
        if(this->m_recvBuf.size()<2)
        {
            break;
        }
        // 返回异常码
        if((quint8)(this->m_recvBuf.at(1)&0X80)==0X80)
        {
            if(this->m_recvBuf.size()<5)
            {
                break;
            }
            // crc 校验检查
            quint16  crc=C_endian::uint16(ORDER_BIG,this->m_recvBuf.at(3),this->m_recvBuf.at(4));
            if(C_CRC::crc16((quint8*)this->m_recvBuf.data(),3)!=crc)
            {
                this->m_recvBuf.remove(0,1);  // 校验错
                continue;
            }
            // 功能码检查
            if((quint8)((this->m_recvBuf.at(1) -0x80)) !=this->m_queryTrans.trans.funcCode)
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
            //emit sig_Error(this->m_queryTrans.transID,this->m_queryTrans.trans.slaveAdr,this->m_queryTrans.trans.funcCode,errCode);

            qDebug()<<"返回异常错误码:"<<this->m_recvBuf.at(2)<<errCode;
            this->m_sta = RTUmaster_IDEL; // 空闲状态
            this->m_recvBuf.clear();
            break;
        }
        // 正常返回：检查功能码
        if((quint8)(this->m_recvBuf.at(1))!=this->m_queryTrans.trans.funcCode)
        {
            this->m_recvBuf.remove(0,1);  // 功能码错
            continue;
        }

        // 按照功能码不同解析数据报
        enumMB_FuncCode funcCode = this->m_queryTrans.trans.funcCode;
        quint16 framLen; //数据帧长
        if(MB_func01==funcCode || MB_func02==funcCode)
        {
            if(this->m_recvBuf.size()<3)
            {
                break;
            }
            quint8 bytes = (quint8)(this->m_recvBuf.at(2));
            if(bytes!=C_mod_protocol::bitToByes(this->m_queryTrans.trans.paraSum))
            {
                this->m_recvBuf.remove(0,1);  // 返回长度字节数不对
                continue;
            }
            framLen = bytes+5;
        }else if(MB_func03==funcCode || MB_func04==funcCode)
        {
            if(this->m_recvBuf.size()<3)
            {
                break;  // 等待继续接收
            }
            quint8 bytes = (quint8)(this->m_recvBuf.at(2));
            if(bytes!=2*this->m_queryTrans.trans.paraSum)
            {
                this->m_recvBuf.remove(0,1);  // 返回长度字节数不对
                continue;
            }
            framLen = bytes+5;
        }else if(MB_func0F==funcCode || MB_func10==funcCode)
        {
            framLen = 8;
        }else
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }
        //等待完整帧
        if(this->m_recvBuf.size()<framLen)
        {
            break;
        }
        // crc 校验
        quint16 crc = C_endian::uint16(ORDER_BIG,this->m_recvBuf.at(framLen-2),this->m_recvBuf.at(framLen-1));
        if(C_CRC::crc16((quint8*)this->m_recvBuf.data(),framLen-2)!=crc)
        {
            this->m_recvBuf.remove(0,1);  // 校验错
            continue;
        }
        // 截取数据帧
        QByteArray tFram;
        tFram =  this->m_recvBuf.left(framLen-2); // 不包括 CRC字
        //处理应答
        this->m_sta = RTUmaster_PROCREPLY;
        if(MB_func01==this->m_queryTrans.trans.funcCode||
           MB_func02==this->m_queryTrans.trans.funcCode)
        {
            this->proc_0X01_0X02(tFram);
        }else if(MB_func03==this->m_queryTrans.trans.funcCode||
                 MB_func04==this->m_queryTrans.trans.funcCode)
        {
            this->proc_0X03_0X04(tFram);
        }else if(MB_func0F==this->m_queryTrans.trans.funcCode||
                 MB_func10==this->m_queryTrans.trans.funcCode)
        {
            this->proc_0X0F_0X10(tFram);
        }
        // 复位空闲状态
        this->m_sta = RTUmaster_IDEL;
        this->m_recvBuf.clear();
        break;
    }
}

void C_MB_RTU_MASTER::proc_0X0F_0X10(QByteArray array)
{
    array.remove(0,2);
    if(array.size() == 4)
    {
        // 返回结果
        MB_ReplyBody res;
        //起始地址
        res.wMulR.adr = C_endian::uint16(ORDER_BIG,array.at(0),array.at(1));
        // 返回寄存器数目
        res.wMulR.sum = C_endian::uint16(ORDER_BIG,array.at(2),array.at(3));
        emit sig_proc(this->m_queryTrans.transID,this->m_queryTrans.trans.slaveAdr,this->m_queryTrans.trans.funcCode,res);
    }
}

// 解析 01 pdu
void C_MB_RTU_MASTER::proc_0X01_0X02(QByteArray array)
{
    array.remove(0,3);
    int bytes = C_mod_protocol::bitToByes(this->m_queryTrans.trans.paraSum);
    if(array.size()==bytes)
    {
        MB_ReplyBody res = C_mod_protocol::proc_01_02(array,this->m_queryTrans.trans.paraSum);
        // 发送解析结果信号
        emit sig_proc(this->m_queryTrans.transID,this->m_queryTrans.trans.slaveAdr,this->m_queryTrans.trans.funcCode,res);
    }
}

// 解析03 PDU 数据
void C_MB_RTU_MASTER::proc_0X03_0X04(QByteArray array)
{ 
    array.remove(0,3);
    int bytes = this->m_queryTrans.trans.paraSum*2;
    if(array.size()==bytes)
    {
        MB_ReplyBody res = C_mod_protocol::proc_03_04(array);
        emit sig_proc(this->m_queryTrans.transID,this->m_queryTrans.trans.slaveAdr,this->m_queryTrans.trans.funcCode,res);
        return;
    }
}

void C_MB_RTU_MASTER::queryCMD(MBRequestTransEx trans)
{
    // 组帧
    QByteArray array;
    array.append(trans.trans.slaveAdr);

    if(MB_func01==trans.trans.funcCode||
       MB_func02==trans.trans.funcCode||
       MB_func03==trans.trans.funcCode||
       MB_func04==trans.trans.funcCode)
    {
         array.append(C_mod_protocol::read_01020304PDU(trans.trans.funcCode,trans.trans.beginAdr,trans.trans.paraSum));
    }else if(MB_func10==trans.trans.funcCode)
    {
        QList<MBregister>regList;
        array.append(C_mod_protocol::write_0X10(trans.trans.beginAdr,trans.trans.paraSum,trans.regList));
    }else
    {
        return;
    }
    quint16 crc = C_CRC::crc16((quint8*)array.data(),array.size());

    quint8 by0;
    quint8 by1;
    C_endian::uint16(ORDER_BIG,crc,by0,by1);
    array.append(by0);
    array.append(by1);

    // 清空接收缓存发送数据
    this->m_recvBuf.clear();
    // 等待应答
    this->m_sta = RTUmaster_WAITREPLY;  // 进入：--等待应答--
    this->m_queryTrans = trans;

    // 发送请求
    this->sendData(array);
    this->m_timerSUM=0;  // 定时计数清零

    QString strT;
    foreach(quint8 ch,array)
    {
        strT+=QString::number(ch,16);
        strT+=" ";
    }
    qDebug()<<"SEND:"<<strT;
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

