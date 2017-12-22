#ifndef C_MB_RTU_MASTER_NODE_H
#define C_MB_RTU_MASTER_NODE_H

#include <QObject>
#include <QList>
#include <QTimer>
#include "c_mb_rtu_master.h"
#include "c_mb_master_trans.h"
#include "c_serialport.h"

// 节点对象类
class C_MB_rtu_master_Node : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_rtu_master_Node(QObject *parent = 0);
    ~C_MB_rtu_master_Node(); // 析构
private:
    C_SerialPort m_serial;
    confCommuNode m_confNode;
    QList<MBRequestTransEx> m_listTrans;  //事务请求信息列表
    C_MB_RTU_MASTER m_rtuMaster;          // mobus 规约模块
    QTimer m_timer; //定时器：扫描请求队列
    QList<C_MB_master_trans *>m_listTransObj;  // trans 对象列表

private:
    void slot_Timer();
    void slot_request(MBRequestTransEx trans); //请求
signals:

public slots:
    void slot_proc(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,MB_ReplyBody body);
    void slot_Error(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,RTU_Master_ErrCode errcode);
public:
    void startServ();
    void stopServ();

    bool setNodeInfo(const confCommuNode &confNode);
    void clear();
};

#endif // C_MB_RTU_MASTER_NODE_H
