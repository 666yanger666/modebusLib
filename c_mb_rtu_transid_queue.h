#ifndef C_MB_RTU_TRANSID_QUEUE_H
#define C_MB_RTU_TRANSID_QUEUE_H

#include <QObject>
#include <QList>
#include "../../../WORK/HLMonitorGit/include/include.h"

// 事务ID队列
class C_MB_rtu_transID_Queue : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_rtu_transID_Queue(QObject *parent = 0);
private:
    QList<MBRequestTransEx> m_listTrans;  //事务请求信息列表
signals:

public slots:
    void slot_request(MBRequestTransEx trans); //请求
};

#endif // C_MB_RTU_TRANSID_QUEUE_H
