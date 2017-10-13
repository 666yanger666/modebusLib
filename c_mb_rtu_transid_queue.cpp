#include "c_mb_rtu_transid_queue.h"

C_MB_rtu_transID_Queue::C_MB_rtu_transID_Queue(QObject *parent) : QObject(parent)
{

}

void C_MB_rtu_transID_Queue::slot_request(MBRequestTransEx trans)
{
    int sum = this->m_listTrans.size();
    for(int i=0;i<sum;i++)
    {
        if(this->m_listTrans.at(i).transID == trans.transID)
        {
            return;
        }
    }

    // 添加到队列
    this->m_listTrans.append(trans);
}
