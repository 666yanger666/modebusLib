#ifndef C_TCP_MASTER_AFFAIR_H
#define C_TCP_MASTER_AFFAIR_H

#include <QObject>
#include "c_mod_protocol.h"
#include <QTimer>

enum TCP_Master_ErrCode
{
    TCPmasterErr_TIMEOUT =0, // 响应超时
    TCPmasterErr_REPLY01 =1, // 非法功能码:服务器不支持的功能码
    TCPmasterErr_REPLY02,    // 非法数据地址:与请求有关
    TCPmasterErr_REPLY03,    // 非法数据值:与请求有关
    TCPmasterErr_REPLY04,    // 服务器故障:执行过程中服务器故障
    TCPmasterErr_REPLY05,    // 确认:耗时操作，返回接收确认
    TCPmasterErr_REPLY06,    // 服务器忙:服务器忙，客户端决定重发请求
    TCPmasterErr_REPLY0A,    // 网关故障:网关路径无效
    TCPmasterErr_REPLY0B,    // 网关故障:目标设备无响应，由网关生成该异常
    TCPmasterErr_UNKNOW      // 未知定义
};

// MODEBUS_TCP  主机模式  请求事务 类
class C_tcp_master_affair : public QObject
{
    Q_OBJECT
public:
    explicit C_tcp_master_affair(QObject *parent = 0);

signals:
    void sig_proc(quint16 affID,quint8 slaveAdr,enumMB_FuncCode fcode,MB_ReplyBody body);
    void sig_Error(quint16 affID,quint8 slaveAdr,enumMB_FuncCode fcode,TCP_Master_ErrCode errcode);
signals:
    void sig_sendData(QByteArray &array);  // 连接到 tcp_master对象中转再连接到 通讯模块
public slots:
private slots:
    void slot_replyTimer();
private:
    quint16 m_affairID;       // 事务ID
    quint8 m_devID;           // 设备单元ID
    QTimer m_replytimer;      // 响应超时定时器

    enumMB_FuncCode m_curFcode;  // 当前请求功能码
    quint16 m_curAdr;         // 当前请求起始地址
    quint16 m_curSum;         // 当前请求数据项数
    quint8  m_byteSum;        // 正常应答字节

    bool m_isIdel;            // 是否通讯状态使用
    QByteArray m_recvBuf;     // 应答数据暂存

private:
    void proc_01(QByteArray &array);
    void proc_02(QByteArray &array);
    void proc_03(QByteArray &array);
    void proc_04(QByteArray &array);
private:
    void sendData(QByteArray data);
public:
    void queryCMD(quint16 afid,quint8 devID,enumMB_FuncCode fcode,quint16 adr,quint16 sum,int timeout); // 请求数据
    void replyData(QByteArray data);
    bool isIdel();
};

#endif // C_TCP_MASTER_AFFAIR_H
