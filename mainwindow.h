#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "c_serialport.h"
#include "c_mb_rtu_master.h"
#include "c_mb_tcp_master.h"
#include "ctcpsockclient.h"
#include "mbtypedef.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainWindow *ui;

    C_SerialPort m_serial;
    C_MB_RTU_MASTER m_RTU_master;
    C_MB_TCP_MASTER m_TCP_master;

    CTcpSockClient m_socket;
};

#endif // MAINWINDOW_H
