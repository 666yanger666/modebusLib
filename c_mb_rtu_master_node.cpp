#include "c_mb_rtu_master_node.h"
#include <QDebug>

C_MB_rtu_master_Node::C_MB_rtu_master_Node(QObject *parent) : QObject(parent)
{
    // rtu_master  <-> serial
    connect(&this->m_rtuMaster,C_MB_RTU_MASTER::sig_sendData,&this->m_serial,&C_SerialPort::writeData);
    connect(&this->m_serial,&C_SerialPort::sig_readData,&this->m_rtuMaster,C_MB_RTU_MASTER::slot_recvData);
    //
    connect(&this->m_timer,&QTimer::timeout,this,C_MB_rtu_master_Node::slot_Timer);

    connect(&this->m_rtuMaster,&C_MB_RTU_MASTER::sig_proc,this,&C_MB_rtu_master_Node::slot_proc);
    connect(&this->m_rtuMaster,&C_MB_RTU_MASTER::sig_Error,this,&C_MB_rtu_master_Node::slot_Error);
}

// 定时扫描事务请求队列服务
void C_MB_rtu_master_Node::slot_Timer()
{
    // 判断modbus驱动是否空闲
    if(!this->m_rtuMaster.isEdel())
    {
        return;
    }

    // 取事务请求队首
    if(this->m_listTrans.isEmpty())
    {
        return;
    }

    this->m_curTrans = this->m_listTrans.first();
    this->m_rtuMaster.queryCMD(this->m_curTrans);
    this->m_listTrans.removeFirst(); // 移出队列头
}

void C_MB_rtu_master_Node::slot_request(MBRequestTransEx trans)
{
    int sum = this->m_listTrans.size();
    for(int i=0;i<sum;i++)
    {
        if(this->m_listTrans.at(i).transID == trans.transID)
        {
            return;
        }
    }

    // 添加到队列
    this->m_listTrans.append(trans);
}

void C_MB_rtu_master_Node::slot_proc(int transID,quint8 slaveAdr, enumMB_FuncCode fcode, MB_ReplyBody body)
{
    if(transID!=this->m_curTrans.transID)
    {
        return;
    }

    if(slaveAdr!=this->m_curTrans.trans.slaveAdr)
    {
        return;
    }

    if(fcode!=this->m_curTrans.trans.funcCode)
    {
        return;
    }

    // 按照transID 查找接收应答的 trans对象
    int sum = this->m_listTransObj.size();
    for(int i=0;i<sum;i++)
    {
        if(this->m_listTransObj.at(i)->transID()==transID)
        {
            this->m_listTransObj[i]->replyProc(transID,slaveAdr,fcode,body);
        }
    }
}

void C_MB_rtu_master_Node::slot_Error(int transID,quint8 slaveAdr, enumMB_FuncCode fcode, RTU_Master_ErrCode errcode)
{
    if(transID!=this->m_curTrans.transID)
    {
        return;
    }

    if(slaveAdr!=this->m_curTrans.trans.slaveAdr)
    {
        return;
    }

    if(fcode!=this->m_curTrans.trans.funcCode)
    {
        return;
    }

    // 按照transID 查找接收应答的 trans对象
    int sum = this->m_listTransObj.size();
    for(int i=0;i<sum;i++)
    {
        if(this->m_listTransObj.at(i)->transID()==transID)
        {
            this->m_listTransObj[i]->replyErr(transID,slaveAdr,fcode,errcode);
        }
    }
}

// 添加一个 事务对象
void C_MB_rtu_master_Node::AddTrans(MBRequestTransInfo info)
{
    C_MB_rtu_master_trans *ptr = new C_MB_rtu_master_trans();
    if(ptr!=NULL)
    {
        ptr->setQueryTrans(info);
    }
    this->m_listTransObj.append(ptr);

    connect(ptr,&C_MB_rtu_master_trans::sig_reuest,this,&C_MB_rtu_master_Node::slot_request);
}

// 启动服务 : 定时扫描事务请求队列
void C_MB_rtu_master_Node::startServ(serialCFG comCfg)
{
    // 启动NODE事务扫描定时器
    this->m_timer.start(1000);

    // 依次启动事务对象服务
    int sum = this->m_listTransObj.size();
    for(int i=0;i<sum;i++)
    {
        this->m_listTransObj[i]->start();
    }

    // 打开串口
    this->m_serial.openCOM(comCfg);
}

// 停止服务
void C_MB_rtu_master_Node::stopServ()
{
    // 关闭串口
    this->m_serial.closeCOM();

    // 依次停止事务对象
    int sum = this->m_listTransObj.size();
    for(int i=0;i<sum;i++)
    {
        this->m_listTransObj[i]->stop();
    }

    // 停止事务扫描定时器
    this->m_timer.stop();
}