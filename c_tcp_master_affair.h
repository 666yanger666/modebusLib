#ifndef C_TCP_MASTER_AFFAIR_H
#define C_TCP_MASTER_AFFAIR_H

#include <QObject>
#include "c_mod_protocol.h"
#include <QTimer>

// MODEBUS_TCP  主机模式  请求事务 类
class C_tcp_master_affair : public QObject
{
    Q_OBJECT
public:
    explicit C_tcp_master_affair(QObject *parent = 0);
signals:
    void sig_proc(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,MB_ReplyBody body);
    void sig_Error(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,TCP_Master_ErrCode errcode);
signals:
    void sig_sendData(QByteArray &array);  // 连接到 tcp_master对象中转再连接到 通讯模块
public slots:
private slots:
    void slot_replyTimer();
private:
    QTimer m_replytimer;      // 响应超时定时器
    bool m_isIdel;            // 是否通讯状态使用(等待挂起状态)
    QByteArray m_recvBuf;     // 应答数据暂存
    MBRequestTransTCPEx m_queryTrans;

    int m_timerSUM;      // 定时计数
private:
    void proc_0X01_0X02(QByteArray &array);
    void proc_0X03_0X04(QByteArray &array);
    void proc_0X0F_0X10(QByteArray array);
private:
    void sendData(QByteArray data);
public:
    void queryCMD(MBRequestTransTCPEx trans); // 请求数据
    void replyData(QByteArray data);
    bool isIdel();
};

#endif // C_TCP_MASTER_AFFAIR_H
