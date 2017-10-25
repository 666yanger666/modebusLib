#ifndef CTCPSOCKCLIENT_H
#define CTCPSOCKCLIENT_H
#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QList>



//TCP client 模块
class CTcpSockClient : public QObject
{
    Q_OBJECT
public:
    explicit CTcpSockClient(QObject *parent = 0);
private:
    QTcpSocket *m_pSock;
private:  //内部信号处理
    void on_sockConnected();
    void on_sockDisconnected();
    void on_sockReceived();
    private slots:
    void on_error(QAbstractSocket::SocketError socketError);

signals:
    void sig_recvData(const QByteArray &array); //接收到数据
    void sig_sockConneted(qint8 res); // 连接结果 1:OK  0:失败
    void sig_sockConnected();  // 以建立连接
    void sig_sockDisconnected(); // 已断开连接
    void sig_sockError(QAbstractSocket::SocketError socketError); //sock报错
    void sig_Debug(QString info);

public:
    bool connectSocket(QString strIP,quint16 nPort,int msecs);
    void disconnectSocket();  //断开连接
    QAbstractSocket::SocketState stateSock(); //连接状态
    void sendSockData(QByteArray &array);  //发送流式数据
    void connectSocket(QString strIP,quint16 nPort);  //连接
};

#endif // CTCPSOCKCLIENT_H

