#ifndef C_MB_TCP_MASTER_H
#define C_MB_TCP_MASTER_H

#include <QObject>
#include "c_mod_protocol.h"
#include <QTimer>
#include "c_tcp_master_affair.h"
#include <QMap>

const int MB_TCP_MAX_AFF_SUM = 10;      // 默认支持最大并发事务数为 10
const int MB_TCP_MAX_AFF_ID  = 1024*10; // 最大事务ID计数值，值超过后值归 1

class C_MB_TCP_MASTER : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_TCP_MASTER(QObject *parent = 0);
signals:
    void sig_sendData(QByteArray &array);  //连接通讯模块 发送数据
signals:
    void sig_proc(quint16 affID,quint8 slaveAdr,MOD_FuncCode fcode,MB_ReplyBody body);
    void sig_Error(quint16 affID,quint8 slaveAdr,MOD_FuncCode fcode,TCP_Master_ErrCode errcode);
public slots:
    void slot_recvData(const QByteArray &array);  // 异步接收数据 槽
private:
    quint16 m_affairID;       //
    quint16 makeAffairID();   // 事务ID生成
    quint8 m_devID;           // 单元标识符(对于IP直接寻址：该项不使用固定位 0xff;
                              //          对于串行链路网关：该项为从机地址)

    QList<C_tcp_master_affair*> m_listAffair; // 事务列表
public:
    void setSlaveAdr(quint8 adr);   //  设置从机地址
    void setTimeOut(int ms);        //  设置响应超时
    void queryCMD(MOD_FuncCode fcode,quint16 adr,quint16 sum,int timeout); // 请求数据
};

#endif // C_MB_TCP_MASTER_H
