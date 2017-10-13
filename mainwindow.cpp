#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "c_crc.h"
#include <QDebug>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

  //  connect(&this->m_serial,&C_SerialPort::sig_readData,&this->m_RTU_master,&C_MB_RTU_MASTER::slot_recvData);
  //  connect(&this->m_RTU_master,&C_MB_RTU_MASTER::sig_sendData,&this->m_serial,&C_SerialPort::writeData);

    connect(&this->m_socket,&CTcpSockClient::sig_recvData,&this->m_TCP_master,&C_MB_TCP_MASTER::slot_recvData);
    connect(&this->m_TCP_master,&C_MB_TCP_MASTER::sig_sendData,&this->m_socket,&CTcpSockClient::sendSockData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(this->m_serial.openCOM("COM3",QSerialPort::Baud9600,QSerialPort::Data8,QSerialPort::OneStop, QSerialPort::NoParity,QSerialPort::NoFlowControl))
    {
        QMessageBox::information(this,tr("提示"),tr("OK！"));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    //this->m_RTU_master.setSlaveAdr(0x08);
    this->m_RTU_master.setTimeOut(3000);
    //this->m_RTU_master.queryCMD(func_03 ,0X0001,1); //
}

void MainWindow::on_pushButton_3_clicked()
{
    QByteArray  array;
    array.append((char)0x08);
    array.append((char)0x83);
    array.append((char)0x03);
 //   array.append((char)0x81);

    qDebug()<<C_CRC::crc16((quint8*)array.data(),array.size());

}

void MainWindow::on_pushButton_4_clicked()
{
    //this->m_RTU_master.setSlaveAdr(0x08);
    this->m_RTU_master.setTimeOut(3000);
    //this->m_RTU_master.queryCMD(func_01 ,0X0001,8); //
}

void MainWindow::on_pushButton_5_clicked()
{
    QByteArray  array;
    array.append((char)0x81);
    short t1 = 0x8001;
    unsigned short t2 =0x8001;

    qDebug()<<t1<<t2<<(quint8)array.at(0)<<(t1==t2);
}

void MainWindow::on_pushButton_6_clicked()
{
    this->m_TCP_master.setSlaveAdr(0x08);
    this->m_TCP_master.queryCMD(MB_func03,0X0001,2,3000); //
}

void MainWindow::on_pushButton_7_clicked()
{
    this->m_socket.connectSocket("127.0.0.1",9100);
}
