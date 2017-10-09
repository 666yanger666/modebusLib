#include "c_mb_tcp_master.h"
#include <QDebug>

C_MB_TCP_MASTER::C_MB_TCP_MASTER(QObject *parent) : QObject(parent)
{
    this->m_affairID = 0;

    for(int i=0;i<MB_TCP_MAX_AFF_SUM;i++)
    {
        this->m_listAffair.append(new C_tcp_master_affair);
        connect(this->m_listAffair.last(),&C_tcp_master_affair::sig_proc,this,&C_MB_TCP_MASTER::sig_proc);
        connect(this->m_listAffair.last(),&C_tcp_master_affair::sig_Error,this,&C_MB_TCP_MASTER::sig_Error);

        connect(this->m_listAffair.last(),&C_tcp_master_affair::sig_sendData,this,&C_MB_TCP_MASTER::sig_sendData);
    }
}

// 数据事务分发：接收到的数据广播给所有事务对象
void C_MB_TCP_MASTER::slot_recvData(const QByteArray &array)
{
    int listSUM = this->m_listAffair.size();
    for(int i=0;i<listSUM;i++)
    {
        this->m_listAffair[i]->replyData(array);
    }
}

// 事务ID 生成 ：递增计数 达到MB_TCP_MAX_AFF_ID时计数归 1
quint16 C_MB_TCP_MASTER::makeAffairID()
{
    if(this->m_affairID>= MB_TCP_MAX_AFF_ID)
    {
        this->m_affairID = 1;
    }
    return this->m_affairID += 1;
}

void C_MB_TCP_MASTER::setSlaveAdr(quint8 adr)
{
    this->m_devID = adr;
}

void C_MB_TCP_MASTER::queryCMD(MOD_FuncCode fcode, quint16 adr, quint16 sum,int timeout)
{
    C_tcp_master_affair *aff = new C_tcp_master_affair(this);
    quint16 affID = this->makeAffairID();

    int listSUM = this->m_listAffair.size();
    for(int i=0;i<listSUM;i++)
    {
        if(this->m_listAffair[i]->isIdel())
        {
            //  发送查询命令帧
            this->m_listAffair[i]->queryCMD(affID,this->m_devID,fcode,adr,sum,timeout);
            return;
        }
    }
}
