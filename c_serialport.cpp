#include "c_serialport.h"
#include <QDebug>

C_SerialPort::C_SerialPort(QObject *parent) : QObject(parent)
{
    m_sum = 0;
    m_byte = 0;
    m_byte_s = 0;
    // 建立数据接收槽
    connect(&this->m_serialPort,&QSerialPort::readyRead,this,&C_SerialPort::slot_readData);
}

// 读数据槽函数
void C_SerialPort::slot_readData()
{
    QByteArray array = this->m_serialPort.readAll();
    emit this->sig_readData(array);

    m_sum += 1;
    m_byte_s = array.size();
    m_byte += array.size();
}

// 打开串口
bool C_SerialPort::openCOM(QString comName, qint32 baudRate, QSerialPort::DataBits dataBits, QSerialPort::StopBits stopBits, QSerialPort::Parity parity, QSerialPort::FlowControl flowControl)
{
    if(this->m_serialPort.isOpen())
    {
        return true;
    }
    this->m_serialPort.setPortName(comName);
    this->m_serialPort.setBaudRate(baudRate);
    this->m_serialPort.setDataBits(dataBits);
    this->m_serialPort.setStopBits(stopBits);
    this->m_serialPort.setParity(parity);
    this->m_serialPort.setFlowControl(flowControl);

    return this->m_serialPort.open(QIODevice::ReadWrite);  // 读写模式

}

bool C_SerialPort::openCOM(serialCFG cfg)
{
    return this->openCOM(cfg.comName,
                         cfg.baudRate,
                         cfg.dataBits,
                         cfg.stopBits,
                         cfg.parity,
                         cfg.flowControl);
}

//关闭串口
void C_SerialPort::closeCOM()
{
    if(this->m_serialPort.isOpen())
    {
        this->m_serialPort.close();
    }
}

//写数据
void C_SerialPort::writeData(QByteArray &array)
{
    if(this->m_serialPort.isOpen())
    {
        this->m_serialPort.write(array);
    }
}

void C_SerialPort::get_count(quint64 &sum, quint32 &byte_s, quint32 &byte)
{
    sum = this->m_sum;
    byte_s = this->m_byte_s;
    byte = this->m_byte;
}

QStringList C_SerialPort::comList()
{
    QStringList comlist;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            comlist.append(serial.portName());
            serial.close();
        }
    }

    return comlist;
}

QStringList C_SerialPort::baudList()
{
    QStringList list;
    list<<"110"<<"300"<<"600"<<"1200"<<"2400"<<"4800"<<"9600"<<"14400"<<"19200"<<"38400"<<"56000"<<"57600"<<"115200"<<"128000"<<"256000";
    return list;
}

QStringList C_SerialPort::dataBList()
{
    QStringList list;
    list<<"8"<<"7"<<"6"<<"5";
    return list;
}

QStringList C_SerialPort::stopBList()
{
    QStringList list;
    list<<"1"<<"1.5"<<"2";
    return list;
}

QStringList C_SerialPort::parityList()
{
    QStringList list;
    list<<"无校验"<<"奇校验"<<"偶校验"<<"标记校验"<<"空格校验";
    return list;
}

QStringList C_SerialPort::flowCtrlList()
{
    QStringList list;
    list<<""<<"300"<<"600"<<"1200"<<"2400"<<"4800"<<"9600"<<"14400"<<"19200"<<"38400"<<"56000"<<"57600"<<"115200"<<"128000"<<"256000";
    return list;
}

QSerialPort::Parity C_SerialPort::strToParity(QString str)
{
    if("无校验"==str)
    {
        return QSerialPort::NoParity;
    }else if("奇校验"==str)
    {
        return QSerialPort::OddParity;
    }else if("偶校验"==str)
    {
        return QSerialPort::EvenParity;
    }else if("标记校验"==str)
    {
        return QSerialPort::MarkParity;
    }else if("空格校验"==str)
    {
        return QSerialPort::SpaceParity;
    }else
    {
        return QSerialPort::NoParity;
    }
}

QString C_SerialPort::parityToStr(QSerialPort::Parity parity)
{

    if(QSerialPort::NoParity==parity)
    {
        return "无校验";
    }
    else if(QSerialPort::OddParity==parity)
    {
        return "奇校验";
    }else if(QSerialPort::EvenParity==parity)
    {
        return "偶校验";
    }else if(QSerialPort::MarkParity==parity)
    {
        return "标记校验";
    }else if(QSerialPort::SpaceParity==parity)
    {
        return "空格校验";
    }else
    {
        return "无校验";
    }
}

QSerialPort::StopBits C_SerialPort::strToStopBits(QString str)
{
    if("1"==str)
    {
        return QSerialPort::OneStop;
    }else if("1.5"==str)
    {
        return QSerialPort::OneAndHalfStop;
    }else if("2"==str)
    {
        return QSerialPort::TwoStop;
    }else
    {
        return QSerialPort::UnknownStopBits;
    }
}

QString C_SerialPort::stopBitsToStr(QSerialPort::StopBits stopBits)
{
    if(QSerialPort::OneStop==stopBits)
    {
        return "1";
    }else if(QSerialPort::OneAndHalfStop==stopBits)
    {
        return "1.5";
    }else if(QSerialPort::TwoStop==stopBits)
    {
        return  "2";
    }else
    {
        return "未知";
    }
}


