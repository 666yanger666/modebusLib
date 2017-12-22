#ifndef C_MB_RTU_MASTER_H
#define C_MB_RTU_MASTER_H

#include <QObject>
#include "c_mod_protocol.h"
#include <QTimer>
#include <QList>
#include "../../../WORK/HLMonitorGit/Fram/WORK/HLMonitor/stable.h"

/*
寄存器地址  连续/非连续  处理  需要上层按照配置做轮询
*/
class C_MB_RTU_MASTER : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_RTU_MASTER(QObject *parent = 0);
private:
    void sendData(QByteArray &array);
signals:
    void sig_proc(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,MB_ReplyBody body);
    void sig_Error(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,RTU_Master_ErrCode errcode);
signals:
    void sig_sendData(QByteArray &array);  //连接通讯模块 发送数据
private:
    int m_timeout;       // 响应超时定时 ms
    int m_retryTimes;    // 重传次数
    int m_retrySum;      // 重传计数
    RTU_MasterSta  m_sta;// 状态机转移状态

    QTimer m_replytimer; // 响应超时定时器
    QByteArray m_recvBuf;// 数据接收缓存

    quint8  m_byteSum;   // 正常应答字节

    MBRequestTransEx m_queryTrans;

private:

public slots:
   void slot_replyTimer();
   void slot_recvData(QByteArray &array);  // 异步接收数据--槽
private:
   void proc_0X01(QByteArray array);
   void proc_0X02(QByteArray array);
   void proc_0X03(QByteArray array);
   void proc_0X04(QByteArray array);
   void proc_0X10(QByteArray array);

public:
    void setTimeOut(int ms);        // 设置响应超时
    void queryCMD(MBRequestTransEx trans); // 请求数据
    bool isIdle();
};

#endif // C_MB_RTU_MASTER_H
