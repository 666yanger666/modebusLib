#ifndef C_MB_TCP_SLAVE_NODE_H
#define C_MB_TCP_SLAVE_NODE_H

#include <QObject>

class C_MB_tcp_slave_Node : public QObject
{
    Q_OBJECT
public:
    explicit C_MB_tcp_slave_Node(QObject *parent = 0);

signals:

public slots:
};

#endif // C_MB_TCP_SLAVE_NODE_H