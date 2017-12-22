#ifndef C_MB_RTU_SLAVE_NODE_H
#define C_MB_RTU_SLAVE_NODE_H
#include "c_serialport.h"
#include "c_mb_rtu_slave.h"

#include <QObject>

// rtu从机节点
class C_MB_rtu_slave_Node : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_rtu_slave_Node(QObject *parent = 0);

signals:

private:
    C_SerialPort m_serial;
    confCommuNode m_confNode;
    C_MB_RTU_SLAVE m_rtuSlave;

public slots:
};

#endif // C_MB_RTU_SLAVE_NODE_H
