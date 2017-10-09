#include "c_serialport.h"
#include <QDebug>

C_SerialPort::C_SerialPort(QObject *parent) : QObject(parent)
{
    // 建立数据接收槽
    connect(&this->m_serialPort,&QSerialPort::readyRead,this,&C_SerialPort::slot_readData);
}

// 读数据槽函数
void C_SerialPort::slot_readData()
{
    QByteArray array = this->m_serialPort.readAll();
    emit this->sig_readData(array);
}

// 打开串口
bool C_SerialPort::openCOM(QString comName, qint32 baudRate, QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits, QSerialPort::Parity parity, QSerialPort::FlowControl flowControl)
{
    this->closeCOM();

    this->m_serialPort.setPortName(comName);
    this->m_serialPort.setBaudRate(baudRate);
    this->m_serialPort.setDataBits(dataBits);
    this->m_serialPort.setStopBits(stopBits);
    this->m_serialPort.setParity(parity);
    this->m_serialPort.setFlowControl(flowControl);

    return this->m_serialPort.open(QIODevice::ReadWrite);  // 读写模式
}

//关闭串口
void C_SerialPort::closeCOM()
{
    this->m_serialPort.clearError();
    this->m_serialPort.clear();
    this->m_serialPort.close();
}

//写数据
void C_SerialPort::writeData(QByteArray &array)
{
    this->m_serialPort.write(array);
}
