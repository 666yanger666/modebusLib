#ifndef C_MB_RTU_MASTER_H
#define C_MB_RTU_MASTER_H

#include <QObject>
#include "c_mod_protocol.h"
#include <QTimer>

// RTU 主机模式 状态机 状态定义
enum RTU_MasterSta
{
    RTUmaster_IDEL =0,     // 空闲
    RTUmaster_CONVDELAY=1, // 转换延迟
    RTUmaster_WAITREPLY,   // 等待应答
    RTUmaster_PROCREPLY,   // 处理应答
    RTUmaster_ERRPROC      // 出错处理
};

//  RTU 主机模式 错误码定义
enum RTU_Master_ErrCode
{
    RTUmasterErr_TIMEOUT =0, // 响应超时
    RTUmasterErr_REPLY01 =1, // 非法功能码 配合功能码
    RTUmasterErr_REPLY02,    // 非法数据地址
    RTUmasterErr_REPLY03,    // 非法数据值
    RTUmasterErr_REPLY04,    // 从站设备故障
};


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
    void sig_proc(quint8 slaveAdr,MOD_FuncCode fcode,MB_ReplyBody body);
    void sig_Error(quint8 slaveAdr,MOD_FuncCode fcode,RTU_Master_ErrCode errcode);
signals:
    void sig_sendData(QByteArray &array);  //连接通讯模块 发送数据
private:
    quint8 m_slaveAdr;   // 从机地址（主站模式：对方从机地址)
    int m_timeout;       // 响应超时定时 ms
    int m_retryTimes;    // 重传次数
    int m_retrySum;      // 重传计数
    RTU_MasterSta  m_sta;// 状态机转移状态

    QTimer m_replytimer; // 响应超时定时器
    QByteArray m_recvBuf;// 数据接收缓存

    MOD_FuncCode m_curFcode;  // 当前请求功能码
    quint16 m_curAdr;         // 当前请求起始地址
    quint16 m_curSum;         // 当前请求数据项数
    quint8  m_byteSum;        // 正常应答字节
private:

public slots:
   void slot_replyTimer();
   void slot_recvData(QByteArray &array);  // 异步接收数据 槽
private:
   void proc_01(QByteArray &array);
   void proc_02(QByteArray &array);
   void proc_03(QByteArray &array);
   void proc_04(QByteArray &array);

public:
    void setSlaveAdr(quint8 adr);   //  设置从机地址
    void setTimeOut(int ms);        //  设置响应超时
    void queryCMD(MOD_FuncCode fcode,quint16 adr,quint16 sum); // 请求数据
};

#endif // C_MB_RTU_MASTER_H
