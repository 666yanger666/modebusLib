#include "c_mb_rtu_slave_node.h"

C_MB_rtu_slave_Node::C_MB_rtu_slave_Node(QObject *parent) : QObject(parent)
{
    // rtu_master  <-> serial
    connect(&this->m_rtuSlave,&C_MB_RTU_SLAVE::sig_sendData,&this->m_serial,&C_SerialPort::writeData);
    connect(&this->m_serial,&C_SerialPort::sig_readData,&this->m_rtuSlave,&C_MB_RTU_SLAVE::slot_recvData);
}
