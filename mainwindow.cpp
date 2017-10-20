#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "c_crc.h"
#include <QDebug>
#include <QMessageBox>
#include <QUuid>

#include <QFile>
#include <QTextStream>
#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptValueList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&this->m_socket,&MODEBUS::CTcpSockClient::sig_recvData,&this->m_TCP_master,&C_MB_TCP_MASTER::slot_recvData);
    connect(&this->m_TCP_master,&C_MB_TCP_MASTER::sig_sendData,&this->m_socket,&MODEBUS::CTcpSockClient::sendSockData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    serialCFG cfg;
    cfg.comName = "COM3";
    cfg.baudRate = QSerialPort::Baud9600;
    cfg.dataBits = QSerialPort::Data8;
    cfg.stopBits = QSerialPort::OneStop;
    cfg.parity =  QSerialPort::NoParity;
    cfg.flowControl = QSerialPort::NoFlowControl;

    MBRequestTransInfo trans;
    trans.trans.slaveAdr = 0x08;
    trans.trans.funcCode = MB_func03;
    trans.trans.beginAdr =0x0001;
    trans.trans.paraSum = 0x01;
    trans.timeGap = 5;
    this->m_RTUmasterNode.AddTrans(trans);

    trans.trans.slaveAdr = 0x08;
    trans.trans.funcCode = MB_func01;
    trans.trans.beginAdr =0x0001;
    trans.trans.paraSum = 0x01;
    trans.timeGap = 5;
    this->m_RTUmasterNode.AddTrans(trans);

    this->m_RTUmasterNode.startServ(cfg);
}

void MainWindow::on_pushButton_2_clicked()
{

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
    //this->m_RTU_master.setTimeOut(3000);
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

void MainWindow::on_pushButton_8_clicked()
{
    for(int i=0;i<1000;i++)
    {
        QUuid ID = QUuid::createUuid();

        qDebug()<< ID.toString();
    }
}

void MainWindow::on_pushButton_9_clicked()
{
       QString result = "";
       QFile scriptFile("TransCal.js");
       if (!scriptFile.open(QIODevice::ReadOnly))
       {
           result.clear();
           qWarning() << "encodePass.js open failed";
           return;
       }
       QTextStream out(&scriptFile);
       QString contents = out.readAll();
       scriptFile.close();
       qWarning()<<"OK";

       contents.clear();
       contents = "function1 trans2wCal(var1,var2)\r\n\
       {\r\n\
               var MAX =200;\r\n\
               var res = var1+var2;\r\n\
               if(res>MAX)\r\n\
               return 0;\r\n\
               return  res;\r\n\
       }\r\n\
               function trans3wCal(var1,var2)\r\n\
       {\r\n\
               return var1*var2+5;\r\n\
       }";

       ui->textEdit->setText(contents);
       QScriptSyntaxCheckResult res =QScriptEngine::checkSyntax (contents);
       qWarning()<<"语法检查:"<<res.state();

       QTextCursor cursor(ui->textEdit->textCursor());//
       cursor.setPosition(res.errorLineNumber());
       if(0==res.state())
       {
           qWarning()<<"错误行:"<<res.errorLineNumber()<<"错误列："<<res.errorColumnNumber();
           qWarning()<<"错误提示："<<res.errorMessage();
           return;
       }

       double var1 =100.5;  // ui->lineEdit_var1->text().toDouble();
       double var2 =200.6;  // ui->lineEdit_var2->text().toDouble();
       qDebug() << "var1:" << var1 << "var2:" << var2;

       QScriptValueList args;      //调用js方法时传入的参数
       args << QScriptValue(var1) << QScriptValue(var2);
       QScriptEngine engine;
       QScriptValue js = engine.evaluate(contents);        //个人理解：加载js文本到操作引擎
   js.isError()
       QScriptValue func;
       func = engine.globalObject().property("trans2wCal");   //调用js方法
       double ret = func.call(QScriptValue(), args).toVariant().toDouble();
       qDebug() << "result:" << ret;
      // ui->lineEdit_sub->setText(result);

       func = engine.globalObject().property("trans3wCal");   //调用js方法
       result = func.call(QScriptValue(), args).toString();
       qDebug() << "result:" << result;
      // ui->lineEdit_mul->setText(result);
}

void MainWindow::on_pushButton_10_clicked()
{
    quint8 t1= 0Xf0;
    qint8 t2= 0Xf0;

    double db= t1;
    qDebug()<<(-2*t1+1)<<(-2*t2+1)<<double(t1)<<db;
}
