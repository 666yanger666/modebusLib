#ifndef C_MB_TCP_MASTER_NODE_H
#define C_MB_TCP_MASTER_NODE_H

#include <QObject>
#include "c_mb_tcp_master.h"
#include "c_mb_master_trans.h"
#include "ctcpsock.h"

class C_MB_tcp_master_Node : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_tcp_master_Node(QObject *parent = 0);
private:
    CTcpSock m_tcpSock;
    confCommuNode m_confNode;
    QList<MBRequestTransEx> m_listTrans;  //事务请求信息列表
    C_MB_TCP_MASTER m_tcpMaster;   // mobus 规约模块
    QTimer m_timer; //定时器：扫描请求队列
    QList<C_MB_master_trans *>m_listTransObj;  // trans 对象列表

    QList<MBRequestTransEx> m_curTransList; // 当前占线transList  记录当前查询事务INFO list
private:
    void slot_Timer();
    void slot_request(MBRequestTransEx trans); //请求
signals:

public slots:
    void slot_proc(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,MB_ReplyBody body);
    void slot_Error(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,RTU_Master_ErrCode errcode);

public:
    void stopServ();

    void startServ();

    bool setNodeInfo(const confCommuNode &confNode);

    void clear();
};

#endif // C_MB_TCP_MASTER_NODE_H
