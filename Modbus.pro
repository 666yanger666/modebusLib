#-------------------------------------------------
#
# Project created by QtCreator 2017-09-11T09:25:51
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Modbus
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    c_serialport.cpp \
    c_crc.cpp \
    c_mod_protocol.cpp \
    c_mb_rtu_master.cpp \
    c_mb_rtu_slave.cpp \
    c_mb_tcp_master.cpp \
    c_mb_tcp_slave.cpp \
    c_tcp_master_affair.cpp \
    ctcpsockclient.cpp \
    c_mb_rtu_master_trans.cpp \
    c_mb_rtu_transid_queue.cpp \
    c_mb_rtu_master_node.cpp

HEADERS  += mainwindow.h \
    c_serialport.h \
    c_crc.h \
    c_mod_protocol.h \
    c_mb_rtu_master.h \
    c_mb_rtu_slave.h \
    c_mb_tcp_master.h \
    c_mb_tcp_slave.h \
    c_tcp_master_affair.h \
    ctcpsockclient.h \
    mbtypedef.h \
    c_mb_rtu_master_trans.h \
    c_mb_rtu_transid_queue.h \
    c_mb_rtu_master_node.h

FORMS    += mainwindow.ui
