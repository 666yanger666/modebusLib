#include "ctcpsockclient.h"
#include <QDebug>
#include<QThread>
#include<QTime>

using namespace MODEBUS;
CTcpSockClient::CTcpSockClient(QObject *parent) : QObject(parent)
{
    m_pSock = new QTcpSocket(this); //必须指定父对象指针

    connect(this->m_pSock,&QTcpSocket::connected,this,&CTcpSockClient::on_sockConnected);
    connect(this->m_pSock,&QTcpSocket::readyRead,this,&CTcpSockClient::on_sockReceived);
    connect(this->m_pSock,&QTcpSocket::disconnected,this,&CTcpSockClient::on_sockDisconnected);
    connect(this->m_pSock,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(on_error(QAbstractSocket::SocketError)));
}

//
void CTcpSockClient::on_sockConnected()
{
    emit sig_sockConnected();
}

void CTcpSockClient::on_sockDisconnected()
{
    emit sig_sockDisconnected();
}

void CTcpSockClient::on_sockReceived()
{
    if(this->m_pSock->bytesAvailable()>0)
    {
       QByteArray array =this->m_pSock->readAll();
       emit sig_recvData(array);
    }
}

void CTcpSockClient::on_error(QAbstractSocket::SocketError socketError)
{
   emit sig_sockError(socketError);
}

// strIP :IP地址
// nPort :端口号
// msecs :超时/ms
bool CTcpSockClient::connectSocket(QString strIP, quint16 nPort,int msecs)
{
    this->m_pSock->abort(); // 取消连接  复位套接字   ... 复位操作
    this->m_pSock->connectToHost(strIP,nPort);

  /*  if(!m_pSock->waitForConnected(msecs))
    {
        emit sig_sockConneted(0); //
        return false;
    }
*/
    if(msecs>0)
    {

    }
    return true;
}

// 断开连接
void CTcpSockClient::disconnectSocket()
{
    if(m_pSock->state()==QAbstractSocket::ConnectedState)
    {
        this->m_pSock->disconnectFromHost();
    }
}

QAbstractSocket::SocketState CTcpSockClient::stateSock()
{
    return this->m_pSock->state();
}

// 发送数据
void CTcpSockClient::sendSockData(QByteArray &array)
{
    this->m_pSock->write(array);
}

void CTcpSockClient::connectSocket(QString strIP, quint16 nPort)
{
    this->m_pSock->abort(); // 取消连接  复位套接字   ... 复位操作
    this->m_pSock->connectToHost(strIP,nPort);
}
