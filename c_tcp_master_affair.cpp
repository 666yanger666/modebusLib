#include "c_tcp_master_affair.h"
#include <QDebug>
#include "c_endian.h"

C_tcp_master_affair::C_tcp_master_affair(QObject *parent) : QObject(parent)
{
    this->m_isIdel =true;
    connect(&this->m_replytimer,&QTimer::timeout,this,&C_tcp_master_affair::slot_replyTimer);

    // 启动应答超时定时器
    this->m_replytimer.start(1000);  //
}

void C_tcp_master_affair::slot_replyTimer()
{
    if(!this->m_isIdel)
    {
        this->m_timerSUM+=1;
        if(this->m_timerSUM>3)
        {
            this->m_recvBuf.clear();
            this->m_isIdel = true;
            emit sig_Error(this->m_queryTrans.trans.transID,this->m_queryTrans.trans.trans.slaveAdr,this->m_queryTrans.trans.trans.funcCode,TCPmasterErr_TIMEOUT);

            qDebug()<<"超时！";
        }
    }
}

void C_tcp_master_affair::proc_0X01_0X02(QByteArray &array)
{
    array.remove(0,9);
    int bytes = C_mod_protocol::bitToByes(this->m_queryTrans.trans.trans.paraSum);
    if(array.size()==bytes)
    {
        MB_ReplyBody res = C_mod_protocol::proc_01_02(array,this->m_queryTrans.trans.trans.paraSum);
        // 发送解析结果信号
        emit sig_proc(this->m_queryTrans.trans.transID,this->m_queryTrans.trans.trans.slaveAdr,this->m_queryTrans.trans.trans.funcCode,res);
    }
}

void C_tcp_master_affair::proc_0X03_0X04(QByteArray &array)
{
    array.remove(0,9);
    int bytes = this->m_queryTrans.trans.trans.paraSum*2;
    if(array.size()==bytes)
    {
        MB_ReplyBody res = C_mod_protocol::proc_03_04(array);
        emit sig_proc(this->m_queryTrans.trans.transID,this->m_queryTrans.trans.trans.slaveAdr,this->m_queryTrans.trans.trans.funcCode,res);
        return;
    }
}

void C_tcp_master_affair::proc_0X0F_0X10(QByteArray array)
{
    array.remove(0,8);
    if(array.size() == 4)
    {
        // 返回结果
        MB_ReplyBody res;
        //起始地址
        res.wMulR.adr = C_endian::uint16(ORDER_BIG,array.at(0),array.at(1));
        // 返回寄存器数目
        res.wMulR.sum = C_endian::uint16(ORDER_BIG,array.at(2),array.at(3));
        emit sig_proc(this->m_queryTrans.trans.transID,this->m_queryTrans.trans.trans.slaveAdr,this->m_queryTrans.trans.trans.funcCode,res);
    }
}

void C_tcp_master_affair::sendData(QByteArray data)
{
    emit sig_sendData(data);
}

void C_tcp_master_affair::queryCMD(MBRequestTransTCPEx trans)
{
    enumMB_FuncCode fcode=trans.trans.trans.funcCode;
    quint16 afid = trans.affairID;
    quint8 devID = trans.trans.trans.slaveAdr;
    quint16 sum = trans.trans.trans.paraSum;
    quint16 adr = trans.trans.trans.beginAdr;

    // 组帧
    QByteArray fram;

    // MBAP
    quint8 by0;
    quint8 by1;
    C_endian::uint16(ORDER_BIG,afid,by0,by1); // 事务标识符
    fram.append(by0);
    fram.append(by1);

    // MB协议标识
    fram.append((char)0x00);
    fram.append((char)0x00);

    QByteArray PDU;  // 数据
    if(MB_func01==fcode ||
       MB_func01==fcode ||
       MB_func03==fcode ||
       MB_func04==fcode)
    {
       PDU= C_mod_protocol::read_01020304PDU(fcode,adr,sum);
    }else if(MB_func10==fcode)
    {
       PDU= C_mod_protocol::write_0X10(adr,sum,trans.trans.regList);
    }else
    {
        return;
    }

    // 长度
    C_endian::uint16(ORDER_BIG,PDU.size()+1,by0,by1);
    fram.append(by0);
    fram.append(by1);
    fram.append(devID);  // 单元标识符
    // PDU
    fram.append(PDU);

    // 保存状态
    this->m_isIdel = false;
    this->m_queryTrans = trans;

    // 发送请求  启动应答超时定时器
    this->sendData(fram);
    this->m_timerSUM=0;  // 定时计数清零

    QString strT;
    foreach(quint8 ch,fram)
    {
        strT+=QString::number(ch,16);
        strT+=" ";
    }
    qDebug()<<"SEND:"<<strT;
}

void C_tcp_master_affair::replyData(QByteArray data)
{
    // 判断是否等待数据
    if(this->m_isIdel)
    {
        return;
    }
    this->m_recvBuf.append(data);

    QString strT;
    foreach(quint8 ch,data)
    {
        strT+=QString::number(ch,16);
        strT+=" ";
    }
    qDebug()<<"RECV:"<<strT;

    for(;;)
    {
        //MBAP头
        if(this->m_recvBuf.size()<7)
        {
            return;
        }
        //事务ID
        quint16 temp = C_endian::uint16(ORDER_BIG,this->m_recvBuf.at(0),this->m_recvBuf.at(1));
        if(temp!=this->m_queryTrans.affairID)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }
        // MB协议标识符
        temp = C_endian::uint16(ORDER_BIG,this->m_recvBuf.at(2),this->m_recvBuf.at(3));
        if(temp!=0)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }
        // 单元标识符
        if((quint8)(this->m_recvBuf.at(6))!=this->m_queryTrans.trans.trans.slaveAdr)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }
        // 总长度
        temp = C_endian::uint16(ORDER_BIG,this->m_recvBuf.at(4),this->m_recvBuf.at(5));
        if(this->m_recvBuf.size()<6+temp)
        {
            return;
        }
        //funcID
        if(this->m_recvBuf.size()<8)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }
        quint8  fcode = this->m_recvBuf.at(7);
        //是否返回异常码
        if((quint8)(fcode&0X80)==0X80)
        {
            // 异常码返回帧长 = 7+2=9
            if(this->m_recvBuf.size()<9)
            {
                this->m_recvBuf.remove(0,1);
                continue;
            }
            // 功能码检查
            if((quint8)(fcode-0x80) !=this->m_queryTrans.trans.trans.funcCode)
            {
                this->m_recvBuf.remove(0,1);  // 功能码错
                continue;
            }
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
            qDebug()<<"异常码:"<<QString::number((quint8)this->m_recvBuf.at(8),16);
            emit sig_Error(this->m_queryTrans.trans.transID,this->m_queryTrans.trans.trans.slaveAdr,this->m_queryTrans.trans.trans.funcCode,errCode);  // 发送错误码应答

            this->m_recvBuf.clear();
            this->m_isIdel = true;
            return;
        }
        // 正常返回：检查功能码
        if(fcode!=this->m_queryTrans.trans.trans.funcCode)
        {
            this->m_recvBuf.remove(0,1);
            continue;
        }
        if(MB_func01==fcode || MB_func02==fcode)
        {
            if(this->m_recvBuf.size()<9)
            {
                this->m_recvBuf.clear();
                this->m_isIdel = true;
                return;
            }
            quint8 bytes = (quint8)(this->m_recvBuf.at(8));
            if(bytes!=C_mod_protocol::bitToByes(this->m_queryTrans.trans.trans.paraSum))
            {
                this->m_recvBuf.clear();
                this->m_isIdel = true;
                return;
            }
            quint16 framLen = 9+bytes; //数据帧长
            if(this->m_recvBuf.size()<framLen)
            {
                this->m_recvBuf.clear();
                this->m_isIdel = true;
                return;
            }
            QByteArray fram = this->m_recvBuf.left(framLen);
            this->proc_0X01_0X02(fram);
        }else if(MB_func03==fcode || MB_func04==fcode )
        {
            if(this->m_recvBuf.size()<9)
            {
                this->m_recvBuf.clear();
                this->m_isIdel = true;
                return;
            }
            quint8 bytes = (quint8)(this->m_recvBuf.at(8));
            if(bytes!=2*this->m_queryTrans.trans.trans.paraSum)
            {
                this->m_recvBuf.clear();
                this->m_isIdel = true;
                return;
            }
            quint16 framLen = 9+bytes; //数据帧长
            if(this->m_recvBuf.size()<framLen)
            {
                this->m_recvBuf.clear();
                this->m_isIdel = true;
                return;
            }
            QByteArray fram = this->m_recvBuf.left(framLen);
            this->proc_0X03_0X04(fram);
        }else if(MB_func0F==fcode||MB_func10==fcode)
        {
            quint16 framLen = 12;
            if(this->m_recvBuf.size()<framLen)
            {
                this->m_recvBuf.clear();
                this->m_isIdel = true;
                return;
            }
            QByteArray fram = this->m_recvBuf.left(framLen);
            this->proc_0X0F_0X10(fram);
        }
        this->m_recvBuf.clear();
        this->m_isIdel = true;
    }
}

bool C_tcp_master_affair::isIdel()
{
    return this->m_isIdel;
}
