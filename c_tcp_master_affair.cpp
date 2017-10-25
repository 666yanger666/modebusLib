#include "c_tcp_master_affair.h"
#include <QDebug>

C_tcp_master_affair::C_tcp_master_affair(QObject *parent) : QObject(parent)
{
    this->m_isIdel =true;
    connect(&this->m_replytimer,&QTimer::timeout,this,C_tcp_master_affair::slot_replyTimer);
}

void C_tcp_master_affair::slot_replyTimer()
{
    this->m_isIdel = true;
    this->m_replytimer.stop();
    this->m_recvBuf.clear();

    // 发送超时
  //  emit sig_Error(this->m_affairID,this->m_devID,this->m_curFcode,TCPmasterErr_TIMEOUT);

    qDebug()<<"超时！";
}

void C_tcp_master_affair::proc_01(QByteArray &array)
{
    if(array.size() != this->m_byteSum)
    {
        return;
    }
    if(this->m_byteSum!=(this->m_queryTrans.trans.trans.paraSum+8)/8)
    {
        return;
    }
    MB_ReplyBody res = C_mod_protocol::proc_01(this->m_byteSum,this->m_queryTrans.trans.trans.paraSum,array);

    // 发送解析结果信号
    emit sig_proc(this->m_queryTrans.trans.transID,this->m_queryTrans.trans.trans.slaveAdr,this->m_queryTrans.trans.trans.funcCode,res);
}

void C_tcp_master_affair::proc_02(QByteArray &array)
{
    if(array.size() != this->m_byteSum)
    {
        return;
    }
    if(this->m_byteSum!=(this->m_queryTrans.trans.trans.paraSum+8)/8)
    {
        return;
    }

    MB_ReplyBody res = C_mod_protocol::proc_02(this->m_byteSum,this->m_queryTrans.trans.trans.paraSum,array);

    emit sig_proc(this->m_queryTrans.trans.transID,this->m_queryTrans.trans.trans.slaveAdr,this->m_queryTrans.trans.trans.funcCode,res);
}

void C_tcp_master_affair::proc_03(QByteArray &array)
{
    if(array.size() != this->m_byteSum)
    {
        return;
    }
    if(this->m_byteSum!=m_queryTrans.trans.trans.paraSum*2)
    {
        return;
    }

    MB_ReplyBody res = C_mod_protocol::proc_03(this->m_queryTrans.trans.trans.paraSum,array);
    foreach(quint16 d,res.reg)
    {
        qDebug()<<"*****"<<QString::number(d,16);
    }

    //发送信号：处理结果
    emit sig_proc(this->m_queryTrans.trans.transID,this->m_queryTrans.trans.trans.slaveAdr,this->m_queryTrans.trans.trans.funcCode,res);
}

void C_tcp_master_affair::proc_04(QByteArray &array)
{
    if(array.size() != this->m_byteSum)
    {
        return;
    }
    if(this->m_byteSum!=m_queryTrans.trans.trans.paraSum*2)
    {
        return;
    }

    MB_ReplyBody res = C_mod_protocol::proc_04(this->m_queryTrans.trans.trans.paraSum,array);
    emit sig_proc(this->m_queryTrans.trans.transID,this->m_queryTrans.trans.trans.slaveAdr,this->m_queryTrans.trans.trans.funcCode,res);
}

void C_tcp_master_affair::sendData(QByteArray data)
{
    emit sig_sendData(data);
}

void C_tcp_master_affair::queryCMD(MBRequestTransTCPEx trans,int timeout)
{
    enumMB_FuncCode fcode=trans.trans.trans.funcCode;
    quint16 afid = trans.affairID;
    quint8 devID = trans.trans.trans.slaveAdr;
    quint16 sum = trans.trans.trans.paraSum;
    quint16 adr = trans.trans.trans.beginAdr;

    if(fcode!=MB_func01 &&
       fcode!=MB_func02 &&
       fcode!=MB_func03 &&
       fcode!=MB_func04)
    {
        return;
    }

    // 组帧
    QByteArray fram;
    def_uin16 temp;

    // MBAP
    temp.value = afid;  // 事务标识符
    fram.append(temp.by_1);
    fram.append(temp.by_0);

    temp.value = 0;     // MB协议标识
    fram.append(temp.by_1);
    fram.append(temp.by_0);

    QByteArray PDU = C_mod_protocol::read_01020304PDU(fcode,adr,sum);
    temp.value = (quint16)(PDU.size()+1); // 长度
    fram.append(temp.by_1);
    fram.append(temp.by_0);

    fram.append(devID);  // 单元标识符

    // PDU
    fram.append(PDU);

    // 计算正常回应数据字节数
    if(MB_func01==fcode)
    {
        this->m_byteSum = (sum+8)/8;
    }else if(MB_func02==fcode)
    {
        this->m_byteSum = (sum+8)/8;
    }else if(MB_func03==fcode)
    {
        this->m_byteSum = sum*2;
    }else if(MB_func04==fcode)
    {
        this->m_byteSum = sum*2;
    }

    // 保存状态
    this->m_isIdel = false;
    this->m_queryTrans = trans;

    // 发送请求  启动应答超时定时器
    this->sendData(fram);
    this->m_replytimer.start(timeout);
}

void C_tcp_master_affair::replyData(QByteArray data)
{
    enumMB_FuncCode fcode=this->m_queryTrans.trans.trans.funcCode;
    quint16 afid = this->m_queryTrans.affairID;
    quint8 devID = this->m_queryTrans.trans.trans.slaveAdr;
    quint16 sum = this->m_queryTrans.trans.trans.paraSum;
    quint16 adr = this->m_queryTrans.trans.trans.beginAdr;
    int transID = this->m_queryTrans.trans.transID;
    // 判断是否等待数据
    if(this->m_isIdel)
    {
        return;
    }
    this->m_recvBuf.append(data);

    for(;;)
    {
        if(this->m_recvBuf.size()<7)
        {
            return;
        }

        def_uin16 temp;
        // 事务ID
        temp.by_1 = this->m_recvBuf.at(0);
        temp.by_0 = this->m_recvBuf.at(1);

        if(afid!=temp.value)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }

        // MB协议标识符
        temp.by_1 = this->m_recvBuf.at(2);
        temp.by_0 = this->m_recvBuf.at(3);
        if(temp.value!=0)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }

        // 单元标识符
        if(devID != this->m_recvBuf.at(6))
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }

        // 长度
        temp.by_1 = this->m_recvBuf.at(4);
        temp.by_0 = this->m_recvBuf.at(5);

        if(this->m_recvBuf.size()<6+temp.value)
        {
            return;
        }

        //funcID
        if(this->m_recvBuf.size()<9)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }
        if(((quint8)this->m_recvBuf.at(7))>=0x80)
        {
            // 解析异常码
            quint8 err = (quint8)this->m_recvBuf.at(8);
            TCP_Master_ErrCode errCode;
            if(err>=0x01 && err<=0x0B)
            {
                errCode = (TCP_Master_ErrCode)err;  // 类型强转
            }else
            {
                errCode = TCPmasterErr_UNKNOW;  // 未知定义
            }
            emit sig_Error(afid,devID,fcode,errCode);
            qDebug()<<"异常码:"<<QString::number((quint8)this->m_recvBuf.at(8),16);
          //  emit sig_Error(transID,devID,fcode,errCode);  // 发送错误码应答

            this->m_replytimer.stop();
            this->m_recvBuf.clear();
            this->m_isIdel = true;

            return;
        }

        if(this->m_recvBuf.at(7)!=fcode)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }

        // bytesum
        if(this->m_recvBuf.at(8)!=this->m_byteSum)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }

        QByteArray dataArray = this->m_recvBuf.mid(9);
        if(MB_func01==fcode)
        {
            this ->proc_01(dataArray);
        }else if(MB_func02==fcode)
        {
            this ->proc_02(dataArray);
        }else if(MB_func03==fcode)
        {
            this ->proc_03(dataArray);
        }else if(MB_func04==fcode)
        {
            this ->proc_04(dataArray);
        }

        this->m_replytimer.stop();
        this->m_recvBuf.clear();
        this->m_isIdel = true;
    }
}

bool C_tcp_master_affair::isIdel()
{
    return this->m_isIdel;
}
