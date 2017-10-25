#include "c_mb_tcp_master_node.h"

C_MB_tcp_master_Node::C_MB_tcp_master_Node(QObject *parent) : QObject(parent)
{
    // tcp_master  <-> socket
    connect(&this->m_tcpMaster,C_MB_TCP_MASTER::sig_sendData,&this->m_tcpSock,&CTcpSock::sendSockData);
    connect(&this->m_tcpSock,&CTcpSock::sig_recvData,&this->m_tcpMaster,C_MB_TCP_MASTER::slot_recvData);

    // 定时器扫描
    connect(&this->m_timer,&QTimer::timeout,this,C_MB_tcp_master_Node::slot_Timer);

    // node节点与master规约模块
    connect(&this->m_tcpMaster,&C_MB_TCP_MASTER::sig_proc,this,&C_MB_tcp_master_Node::slot_proc);
  //  connect(&this->m_tcpMaster,&C_MB_TCP_MASTER::sig_Error,this,&C_MB_tcp_master_Node::slot_Error);
    connect(&this->m_tcpMaster,&C_MB_TCP_MASTER::sig_Error,this,&C_MB_tcp_master_Node::slot_Error);
}

// 定时扫描事务请求队列服务
void C_MB_tcp_master_Node::slot_Timer()
{
    // 判断modbus驱动是否有空闲的事务位置
    if(!this->m_tcpMaster.hasIdle())
    {
        return;
    }

    // 取事务请求队首
    if(this->m_listTrans.isEmpty())
    {
        return;
    }

    this->m_tcpMaster.queryCMD(this->m_listTrans.first());
    this->m_listTrans.removeFirst(); // 移出队列头
}

void C_MB_tcp_master_Node::slot_request(MBRequestTransEx trans)
{

}

void C_MB_tcp_master_Node::slot_proc(int transID, quint8 slaveAdr, enumMB_FuncCode fcode, MB_ReplyBody body)
{

}

void C_MB_tcp_master_Node::slot_Error(int transID, quint8 slaveAdr, enumMB_FuncCode fcode, RTU_Master_ErrCode errcode)
{

}


void C_MB_tcp_master_Node::AddTrans(MBRequestTransInfo info)
{
    C_MB_tcp_master_trans *ptr = new C_MB_tcp_master_trans();
    if(ptr!=NULL)
    {
        ptr->setQueryTrans(info);
    }
    this->m_listTransObj.append(ptr);

    connect(ptr,&C_MB_tcp_master_trans::sig_reuest,this,&C_MB_tcp_master_Node::slot_request);
}

void C_MB_tcp_master_Node::startServ(tcpServCFG tcpCfg)
{
    // 启动NODE事务扫描定时器
    this->m_timer.start(1000);

    // 依次启动事务对象服务
    int sum = this->m_listTransObj.size();
    for(int i=0;i<sum;i++)
    {
        this->m_listTransObj[i]->start();
    }

    // 连接tcpServ  (... 连接结果)
    this->m_tcpSock.connectSocket(tcpCfg.strIP,tcpCfg.nPort);
}

void C_MB_tcp_master_Node::stopServ()
{
    // 关闭串口
    this->m_tcpSock.disconnectSocket();

    // 依次停止事务对象
    int sum = this->m_listTransObj.size();
    for(int i=0;i<sum;i++)
    {
        this->m_listTransObj[i]->stop();
    }

    // 停止事务扫描定时器
    this->m_timer.stop();
}
