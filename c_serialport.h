#ifndef C_SERIALPORT_H
#define C_SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

// 串口通讯封装
struct serialCFG
{
    QString comName;    // 端口
    qint32  baudRate;   // 波特率
    enum QSerialPort::DataBits  dataBits;       // 数据位  5，6，7，8
    enum QSerialPort::StopBits stopBits;        // 停止位  1, 1.5, 2
    enum QSerialPort::Parity   parity;          // 校验位  无，奇，偶，标记，空格
    enum QSerialPort::FlowControl flowControl = QSerialPort::NoFlowControl;  // 流控 默认 QSerialPort::NoFlowControl
};
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

};

#endif // C_SERIALPORT_H
