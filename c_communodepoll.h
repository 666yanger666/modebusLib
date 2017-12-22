#ifndef C_COMMUNODEPOLL_H
#define C_COMMUNODEPOLL_H

#include <QObject>
#include "../../../WORK/HLMonitorGit/include/include.h"

struct commuNodeCon
{
    QString nodeName;
    enumCommuNodeType nodeType;
    void *pNode=NULL;   //根据节点类型转换对象类型，实现接口统一
};

// commNode 对象管理池
class C_commuNodePoll : public QObject
{
    Q_OBJECT
public:
    explicit C_commuNodePoll(QObject *parent = 0);

signals:

public slots:

private:
   static QList<commuNodeCon>m_nodeList; // 动态节点列表
private:
   static bool setconfCommuNodeList(const QList<confCommuNode> &nodeList);
   static void clear();
public:
   static bool initeNodeList();
   static void startServ();  // 启动服务
   static void stop();  // 停止服务


  // static void startServ(QString nodeName);
  // static void stop(QString nodeName);
};

#endif // C_COMMUNODEPOLL_H
