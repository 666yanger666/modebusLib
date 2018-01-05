#ifndef C_MB_RTU_SLAVE_H
#define C_MB_RTU_SLAVE_H

#include <QObject>
#include "c_mod_protocol.h"
#include "../../../WORK/HLMonitorGit/include/include.h"

class C_MB_RTU_SLAVE : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_RTU_SLAVE(QObject *parent = 0);

signals:
    void sig_sendData(QByteArray &array);  //连接通讯模块 发送数据
public slots:
    void slot_recvData(QByteArray &array); //异步接收数据 槽
private:
    RTU_SlaveSta m_sta;  // 状态机状态定义
    quint8 m_slaveAdr;   // 从机地址（主站模式：本机地址)
    QByteArray m_recvBuf;// 数据接收缓存

    RTUSlaveNodeInfo m_slaveInfo;

private:
   void proc_Fram(enumMB_FuncCode fcode,QByteArray &fram);
   void proc_01(QByteArray &array); // 读线圈       最大数量0X7D0 - 2000
   void proc_02(QByteArray &array); // 读输入数字量  最大数量0X7D0 - 2000
   void proc_03(QByteArray &array); // 读寄存器     最大数量0X7D  - 125
   void proc_04(QByteArray &array); // 读输入寄存器  最大数量0X7D  - 125
   void proc_05(QByteArray &array); // 写单个线圈    ON:0XFF00  OFF:0X0000
   void proc_06(QByteArray &array); // 写单个寄存器
   void prco_0F(QByteArray &array); // 写多个线圈    最大数量0X7B0 - 1968
   void proc_10(QByteArray &array); // 写多个寄存器  最大数量 0X7B-123

   QList<bool> m_regDataCFG;

   void Error_Reply(quint8 slaveAdr,enumMB_FuncCode fcode,RTU_Slave_ErrCode errCode);

private:
    void sendData(QByteArray &array);
};

#endif // C_MB_RTU_SLAVE_H
