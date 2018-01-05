#ifndef C_MB_TCP_MASTER_H
#define C_MB_TCP_MASTER_H

#include <QObject>
#include "c_mod_protocol.h"
#include "c_tcp_master_affair.h"
#include "../../../WORK/HLMonitorGit/include/include.h"
#include <QMap>

// TCP Master主机规约
const int MB_TCP_MAX_AFF_SUM = 1;      // 默认支持最大并发事务数为 10
const int MB_TCP_MAX_AFF_ID  = 1024*10; // 最大事务ID计数值，值超过后值归 1

class C_MB_TCP_MASTER : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_TCP_MASTER(QObject *parent = 0);
signals:
    void sig_sendData(QByteArray &array);  //连接通讯模块 发送数据
signals:
    void sig_proc(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,MB_ReplyBody body);
    void sig_Error(int transID,quint8 slaveAdr,enumMB_FuncCode fcode,TCP_Master_ErrCode errcode);
public slots:
    void slot_recvData(const QByteArray &array);  // 异步接收数据槽（从socket接收）
private:
    quint16 m_affairID;       //
    quint16 makeAffairID();   // 事务ID生成
    quint8 m_devID;           // 单元标识符(对于IP直接寻址：该项使用固定值 0xff;
                              //          对于串行链路网关：该项为从机地址)
    QList<C_tcp_master_affair*> m_listAffair; // 事务对象列表
public:
    void queryCMD(MBRequestTransEx trans); // 请求数据
    bool hasIdle();
};

#endif // C_MB_TCP_MASTER_H

/*
MODBUS-TCP 规约摘录
 * 4种节点：MBTCP客户机、MBTCP服务器、MBTCP客户机网关、MBTCP服务器网关
 *实现规则：
1)如果没有明确的用户需求，建议采用自动的 TCP 连接管理
2)建议：打开并保持与远端设备的连接，而不要在每次 MODBUS/TCP 事务处理时打开和关闭连接。
  注：然而，MODBUS 客户必须能够接收来自服务器的关闭请求，并关闭连接。当需要时，连接可以
  被重新打开
3)建议：每一个 MODBUS 客户至少要打开与远端 MODBUS 服务器的 TCP 连接（同一 IP 地址）。
  一个应用建立一个连接是好的选择
4)几个 MODBUS 事务处理可以在同一个 TCP 连接上被同时激活
  注：如果以此方式，MODBUS 事务处理标识必须被用来唯一地识别请求与响应的匹配
5)在两个远端 MODBUS 设备（一个客户机和一个服务器）之间双向通信的情况下，有必要为客户机
  数据流和服务器数据流分别建立连接。
6)一个 TCP 帧只能传送一个 MODBUS ADU。建议：不要在同一个 TCP PDU 中发送多个请求或应答。
*/
