#ifndef C_MB_RTU_MASTER_TRANS_H
#define C_MB_RTU_MASTER_TRANS_H

#include <QObject>
#include <QTimer>
#include "../../../WORK/HLMonitorGit/include/include.h"

// rtu master 事务对象：自带定时器  实现定时请求
class C_MB_rtu_master_trans : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_rtu_master_trans(QObject *parent = 0);

private:
    static int m_indexID;
    QTimer m_timer;
    MBRequestTransInfo m_trans;
    int m_transID;

    QList<MBInputParaCFG>m_listParaCFG;

private slots:
    void slot_timer();
signals:
    void sig_reuest(MBRequestTransEx trans);
public slots:

public:
    void setQueryTrans(const MBRequestTransInfo &trans);
    void start();
    void stop();
    int transID();
    void replyProc(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,MB_ReplyBody body);
    void replyErr(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,RTU_Master_ErrCode errcode);
};

#endif // C_MB_RTU_MASTER_TRANS_H
