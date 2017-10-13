#ifndef C_MB_RTU_SLAVE_H
#define C_MB_RTU_SLAVE_H

#include <QObject>
#include "c_mod_protocol.h"
#include "mbtypedef.h"

class C_MB_RTU_SLAVE : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_RTU_SLAVE(QObject *parent = 0);

signals:
    void sig_sendData(QByteArray &array);  //连接通讯模块 发送数据
public slots:
    void slot_recvData(QByteArray &array);  // 异步接收数据 槽
private:
    RTU_SlaveSta m_sta;
    quint8 m_slaveAdr;   // 从机地址（主站模式：本机地址)

    QByteArray m_recvBuf;// 数据接收缓存

private:
   void proc_01(QByteArray &array);
   void proc_02(QByteArray &array);
   void proc_03(QByteArray &array);
   void proc_04(QByteArray &array);


   QList<bool> m_regDataCFG;

private:
    void sendData(QByteArray &array);
};

#endif // C_MB_RTU_SLAVE_H
