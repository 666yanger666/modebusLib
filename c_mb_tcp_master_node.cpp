#include "c_mb_tcp_master_node.h"

C_MB_tcp_master_Node::C_MB_tcp_master_Node(QObject *parent) : QObject(parent)
{
    // tcp_master  <-> socket
    connect(&this->m_tcpMaster,&C_MB_TCP_MASTER::sig_sendData,&this->m_tcpSock,&CTcpSock::sendSockData);
    connect(&this->m_tcpSock,&CTcpSock::sig_recvData,&this->m_tcpMaster,&C_MB_TCP_MASTER::slot_recvData);

    // 定时器扫描
    connect(&this->m_timer,&QTimer::timeout,this,&C_MB_tcp_master_Node::slot_Timer);

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

void C_MB_tcp_master_Node::slot_proc(int transID, quint8 slaveAdr, enumMB_FuncCode fcode, MB_ReplyBody body)
{
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

void C_MB_tcp_master_Node::slot_Error(int transID, quint8 slaveAdr, enumMB_FuncCode fcode, RTU_Master_ErrCode errcode)
{
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

void C_MB_tcp_master_Node::startServ()
{
    // 连接tcpServ  (... 连接结果)
    this->m_tcpSock.connectSocket(this->m_confNode.tcpMasterNode.nodeInfo.tcpServInfo.strIP,this->m_confNode.tcpMasterNode.nodeInfo.tcpServInfo.nPort);

    // 启动NODE事务扫描定时器
    if(!this->m_timer.isActive())
    {
        this->m_timer.start(500);
    }

    // 依次启动事务对象服务
    int sum = this->m_listTransObj.size();
    for(int i=0;i<sum;i++)
    {
        this->m_listTransObj[i]->start();
    }
}

bool C_MB_tcp_master_Node::setNodeInfo(const confCommuNode &confNode)
{
    this->m_confNode = confNode;
    int sum = confNode.tcpMasterNode.listTrans.size();
    for(int i=0;i<sum;i++)
    {
        C_MB_master_trans *ptr = new C_MB_master_trans;
        if(ptr!=NULL)
        {
            ptr->setQueryTrans(confNode.tcpMasterNode.listTrans.at(i));
            this->m_listTransObj.append(ptr);

            connect(ptr,&C_MB_master_trans::sig_reuest,this,&C_MB_tcp_master_Node::slot_request);
        }else
        {
            return false;
        }
    }
    return true;
}

void C_MB_tcp_master_Node::clear()
{
    foreach(C_MB_master_trans* ptr,this->m_listTransObj)
    {
        delete ptr;
    }

    this->m_listTransObj.clear();
}
