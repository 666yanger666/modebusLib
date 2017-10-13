#ifndef C_MB_RTU_MASTER_NODE_H
#define C_MB_RTU_MASTER_NODE_H

#include <QObject>
#include <QList>
#include <QTimer>
#include "c_mb_rtu_master.h"
#include "c_mb_rtu_master_trans.h"
#include "c_mb_rtu_transid_queue.h"

// 节点对象类
class C_MB_rtu_master_Node : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_rtu_master_Node(QObject *parent = 0);
private:
    QList<MBRequestTransEx> m_listTrans;  //事务请求信息列表
    C_MB_RTU_MASTER m_rtuMaster;   // mobus 规约模块
    QTimer m_timer; //定时器：扫描请求队列
    QList<C_MB_rtu_master_trans *>m_listTransObj;  // trans 对象列表

    MBRequestTransEx m_curTrans; // 当前占线trans
private:
    void slot_Timer();
signals:

public slots:
    void slot_request(MBRequestTransEx trans); //请求
    void slot_proc(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,MB_ReplyBody body);
    void slot_Error(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,RTU_Master_ErrCode errcode);
public:
    void AddTrans(MBRequestTransInfo info);  // 添加一个事务
    void startServ();
    void stopServ();
};

#endif // C_MB_RTU_MASTER_NODE_H
