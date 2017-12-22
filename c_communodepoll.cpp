#include "c_communodepoll.h"
#include "../../../Config/Config/c_configcm.h"
#include "c_mb_tcp_master_node.h"
#include "c_mb_rtu_master_node.h"

QList<commuNodeCon> C_commuNodePoll::m_nodeList;
C_commuNodePoll::C_commuNodePoll(QObject *parent) : QObject(parent)
{

}

//读取配置信息，生成实时通讯对象树
bool C_commuNodePoll::initeNodeList()
{
    C_commuNodePoll::clear();  // 清空
    QList<confCommuNode>nodeList;
    if(C_ConfigCM::getCommuNodeList(nodeList))
    {
        if(!C_commuNodePoll::setconfCommuNodeList(nodeList))
        {
            C_commuNodePoll::clear();
            return false;
        }
    }
    return true;
}

// 根据配置生成实时通讯对象
bool C_commuNodePoll::setconfCommuNodeList(const QList<confCommuNode>&nodeList)
{
    int sum = nodeList.size();
    for(int i=0;i<sum;i++)
    {
        confCommuNode confNode = nodeList.at(i);
        commuNodeCon nodeCon;
        nodeCon.nodeName = confNode.nodeInfo.nodeName;
        nodeCon.nodeType = confNode.nodeInfo.nodeType;

        if(commNode_RTU_MSATER==confNode.nodeInfo.nodeType)
        {
            C_MB_rtu_master_Node *ptr = new C_MB_rtu_master_Node;
            if(ptr!=NULL)
            {
                if(ptr->setNodeInfo(confNode))
                {
                    nodeCon.pNode = ptr;
                    C_commuNodePoll::m_nodeList.append(nodeCon);
                }else
                {
                    delete ptr;
                    C_commuNodePoll::clear();
                    return true;
                }
            }else
            {
                C_commuNodePoll::clear();
                return true;
            }
        }else if(commNode_TCP_MSATER==confNode.nodeInfo.nodeType)
        {
            C_MB_tcp_master_Node *ptr = new C_MB_tcp_master_Node;
            if(ptr!=NULL)
            {
                if(ptr->setNodeInfo(confNode))
                {
                    nodeCon.pNode = ptr;
                    C_commuNodePoll::m_nodeList.append(nodeCon);
                }else
                {
                    delete ptr;
                    C_commuNodePoll::clear();
                    return true;
                }
            }else
            {
                C_commuNodePoll::clear();
                return true;
            }
        }
    }
    return true;
}

// 启动所有通讯节点服务
void C_commuNodePoll::startServ()
{
    int sum = C_commuNodePoll::m_nodeList.size();
    for(int i=0;i<sum;i++)
    {
        commuNodeCon node = C_commuNodePoll::m_nodeList.at(i);
        if(node.pNode!=NULL)
        {
            if(commNode_RTU_MSATER == node.nodeType)
            {
                ((C_MB_rtu_master_Node*)(node.pNode))->startServ();
            }else if(commNode_TCP_MSATER)
            {
                ((C_MB_tcp_master_Node*)(node.pNode))->startServ();
            }
        }
    }
}

void C_commuNodePoll::stop()
{
    int sum = C_commuNodePoll::m_nodeList.size();
    for(int i=0;i<sum;i++)
    {
        commuNodeCon node = C_commuNodePoll::m_nodeList.at(i);
        if(node.pNode!=NULL)
        {
            if(commNode_RTU_MSATER == node.nodeType)
            {
                ((C_MB_rtu_master_Node*)(node.pNode))->stopServ();
            }else if(commNode_TCP_MSATER)
            {
                ((C_MB_tcp_master_Node*)(node.pNode))->stopServ();
            }
        }
    }
}

//
void C_commuNodePoll::clear()
{
    foreach(commuNodeCon node,C_commuNodePoll::m_nodeList)
    {
        if(commNode_RTU_MSATER ==node.nodeType)
        {
            C_MB_rtu_master_Node* ptr = (C_MB_rtu_master_Node*)(node.pNode);
            ptr->clear();
            delete ptr;
        }else if(commNode_TCP_MSATER==node.nodeType)
        {
            C_MB_tcp_master_Node* ptr = (C_MB_tcp_master_Node*)(node.pNode);
            ptr->clear();
            delete ptr;
        }
    }
    C_commuNodePoll::m_nodeList.clear();
}
