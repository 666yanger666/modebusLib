#ifndef C_SERIALPORT_H
#define C_SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "../../../WORK/HLMonitorGit/include/include.h"

class C_SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit C_SerialPort(QObject *parent = 0);

private:
    QSerialPort  m_serialPort;

    quint64 m_sum;
    quint32 m_byte;
    quint32 m_byte_s;
signals:
    void sig_readData(QByteArray &array);
private slots:
    void slot_readData();
public slots:

public:
    bool openCOM( QString comName
                  ,qint32  baudRate
                  ,enum QSerialPort::DataBits  dataBits
                  ,enum QSerialPort::StopBits stopBits
                  ,enum QSerialPort::Parity   parity
                  ,enum QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl);
    bool openCOM(serialCFG cfg);
    void closeCOM();
    void writeData(QByteArray &array);
    void get_count(quint64 &sum,quint32 &byte_s,quint32 &byte);
    static QStringList comList();
    static QStringList baudList();
    static QStringList dataBList();
    static QStringList stopBList();
    static QStringList parityList();
    static QStringList flowCtrlList();
    static enum QSerialPort::Parity strToParity(QString str);
    static QString parityToStr(enum QSerialPort::Parity parity);
    static enum QSerialPort::StopBits strToStopBits(QString str);
    static QString stopBitsToStr(enum QSerialPort::StopBits stopBits);

};

#endif // C_SERIALPORT_H
