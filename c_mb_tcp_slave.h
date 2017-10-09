#ifndef C_MB_TCP_SLAVE_H
#define C_MB_TCP_SLAVE_H

#include <QObject>

class C_MB_TCP_SLAVE : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_TCP_SLAVE(QObject *parent = 0);

signals:
    void sig_sendData(QByteArray &array);   // 连接通讯模块 发送数据
private slots:
    void slot_recvData(QByteArray &array);  // 异步接收数据 槽
public slots:

private:
    quint8 m_devID;           // 单元标识符(对于IP直接寻址：该项不使用固定位 0xff;
                              //          对于串行链路网关：该项为从机地址)

    QByteArray m_recvBuf;// 数据接收缓存

private:
   void proc_01(QByteArray &array);
   void proc_02(QByteArray &array);
   void proc_03(QByteArray &array);
   void proc_04(QByteArray &array);
};

#endif // C_MB_TCP_SLAVE_H
