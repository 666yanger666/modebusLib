#include "c_mb_tcp_master_trans.h"

int C_MB_tcp_master_trans::m_indexID = 0;

C_MB_tcp_master_trans::C_MB_tcp_master_trans(QObject *parent) : QObject(parent)
{
    this->m_transID = ++C_MB_tcp_master_trans::m_indexID;

    connect(&this->m_timer,&QTimer::timeout,this,&C_MB_tcp_master_trans::slot_timer);
}

void C_MB_tcp_master_trans::slot_timer()
{
    MBRequestTransEx trans;
    trans.trans = this->m_trans.trans;
    trans.transID = this->m_transID;
    emit sig_reuest(trans);  // 定时发送总线请求信号
}

void C_MB_tcp_master_trans::setQueryTrans(const MBRequestTransInfo &trans)
{
    this->m_trans = trans;
}

void C_MB_tcp_master_trans::start()
{
    if(!this->m_timer.isActive())
    {
        this->m_timer.start(this->m_trans.timeGap*1000);
    }
}

void C_MB_tcp_master_trans::stop()
{
    if(this->m_timer.isActive())
    {
        this->m_timer.stop();
    }
}

int C_MB_tcp_master_trans::transID()
{
    return this->m_transID;
}

//正常应答
void C_MB_tcp_master_trans::replyProc(int transID, quint8 slaveAdr, enumMB_FuncCode fcode, MB_ReplyBody body)
{
    if(transID!=this->m_transID)
    {
        return;
    }
    if(slaveAdr!=this->m_trans.trans.slaveAdr)
    {
        return;
    }
    if(fcode!=this->m_trans.trans.funcCode)
    {
        return;
    }

    // ... 解析数据
    QString strT;
    qDebug()<<"********"<<transID<<slaveAdr<<fcode;
    body.reg.clear();
}

void C_MB_tcp_master_trans::replyErr(int transID, quint8 slaveAdr, enumMB_FuncCode fcode, RTU_Master_ErrCode errcode)
{
    transID =transID;
    slaveAdr =slaveAdr;
    fcode = fcode;
    errcode = errcode;
}
