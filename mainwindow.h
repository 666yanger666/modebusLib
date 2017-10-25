#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ctcpsock.h"
#include "../../../WORK/HLMonitorGit/include/include.h"

#include "c_mb_tcp_master.h"

#include "c_mb_rtu_master_node.h"

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

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

private:
    Ui::MainWindow *ui;

    C_MB_rtu_master_Node m_RTUmasterNode;
    C_MB_TCP_MASTER m_TCP_master;

    CTcpSock m_socket;
};

#endif // MAINWINDOW_H
