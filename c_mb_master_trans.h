#ifndef C_MB_RTU_MASTER_TRANS_H
#define C_MB_RTU_MASTER_TRANS_H

#include <QObject>
#include <QTimer>
#include "../../../../HLMonitorGit/include/include.h"

// rtu master 事务对象：自带定时器  实现定时请求
class C_MB_master_trans : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_master_trans(QObject *parent = 0);

private:
    static int m_indexID;
    QTimer m_timer;
    confMasterTrans m_trans;
    int m_transID;   // 程序运行动态生成：标识当前trans对象  GUID 做配置用

private slots:
    void slot_timer();
signals:
    void sig_reuest(MBRequestTransEx trans);
public slots:

private:
    bool getMonData(QString unitGUID,QString collGUID,QString monInName,MBOutputParaV &data);
public:
    void setQueryTrans(const confMasterTrans &trans);
    void start();
    void stop();
    int transID();
    void replyProc(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,MB_ReplyBody body);
    void replyErr(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,RTU_Master_ErrCode errcode);
};

#endif // C_MB_RTU_MASTER_TRANS_H
