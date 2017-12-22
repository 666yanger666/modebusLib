#include "c_mb_master_trans.h"
#include <QDebug>
#include "c_endian.h"

int C_MB_master_trans::m_indexID = 0;

C_MB_master_trans::C_MB_master_trans(QObject *parent) : QObject(parent)
{
    this->m_transID = C_MB_master_trans::m_indexID;
    C_MB_master_trans::m_indexID+=1;

    connect(&this->m_timer,&QTimer::timeout,this,&C_MB_master_trans::slot_timer);
}

void C_MB_master_trans::slot_timer()
{
    MBRequestTransEx trans;
    trans.trans = this->m_trans.transInfo.trans;
    trans.transID = this->m_transID;

    if(MB_func10==trans.trans.funcCode)//   写操作
    {
        int sumPara = this->m_trans.transInfo.trans.paraSum;
        for(int i=0;i<sumPara;i++)
        {
            MBregister reg;
            reg.byte_0=0;
            reg.byte_1=0;
            trans.regList.append(reg);
        }
        foreach(MBOutputParaCFG cfg,this->m_trans.outListpara)
        {
            MBOutputParaV dataV;
            if(!this->getMonData(cfg.unitGUID,cfg.collGUID,cfg.monInName,dataV))
            {
                continue;
            }
            if(mbPara_SWT==dataV.paraType) // 如果是开关量  转化为 0，1 ！！！
            {
                continue;  //不作处理
            }
            int dex = cfg.paraADC.paraAdr-this->m_trans.transInfo.trans.beginAdr;
            if(dex<0)
            {
                continue;
            }

            // ADC类型判断
            if(dataV.paraType!=mbPara_ADC)
            {
                continue;
            }

            // 线性变换
            dataV.dataADC.fData64 = dataV.dataADC.fData64 * cfg.paraADC.factor +cfg.paraADC.offset;

            if(MB_paraREAL32 == cfg.paraADC.dataType)
            {
                if(trans.regList.size()>=dex+2)
                {
                    float temp = dataV.dataADC.fData64;
                    quint8 by0,by1,by2,by3;
                    C_endian::float32(cfg.paraADC.boType,temp,by0,by1,by2,by3);
                    trans.regList[dex+0].byte_0=by0;
                    trans.regList[dex+0].byte_1=by1;
                    trans.regList[dex+1].byte_0=by2;
                    trans.regList[dex+1].byte_1=by3;
                }
            }else if(MB_paraREAL64==cfg.paraADC.dataType)
            {
                if(trans.regList.size()>=dex+4)
                {
                    double temp = dataV.dataADC.fData64;
                    quint8 by0,by1,by2,by3,by4,by5,by6,by7;
                    C_endian::float64(cfg.paraADC.boType,temp,by0,by1,by2,by3,by4,by5,by6,by7);
                    trans.regList[dex+0].byte_0=by0;
                    trans.regList[dex+0].byte_1=by1;
                    trans.regList[dex+1].byte_0=by2;
                    trans.regList[dex+1].byte_1=by3;
                    trans.regList[dex+3].byte_0=by4;
                    trans.regList[dex+3].byte_1=by5;
                    trans.regList[dex+4].byte_0=by6;
                    trans.regList[dex+4].byte_1=by7;
                }
            }else if(MB_paraINT32==cfg.paraADC.dataType)
            {
                if(trans.regList.size()>=dex+2)
                {
                    qint32 temp = dataV.dataADC.fData64;
                    quint8 by0,by1,by2,by3;
                    C_endian::int32(cfg.paraADC.boType,temp,by0,by1,by2,by3);
                    trans.regList[dex+0].byte_0=by0;
                    trans.regList[dex+0].byte_1=by1;
                    trans.regList[dex+1].byte_0=by2;
                    trans.regList[dex+1].byte_1=by3;
                }
            }else if(MB_paraUINT32==cfg.paraADC.dataType)
            {
                if(trans.regList.size()>=dex+2)
                {
                    quint32 temp = dataV.dataADC.fData64;
                    quint8 by0,by1,by2,by3;
                    C_endian::uint32(cfg.paraADC.boType,temp,by0,by1,by2,by3);
                    trans.regList[dex+0].byte_0=by0;
                    trans.regList[dex+0].byte_1=by1;
                    trans.regList[dex+1].byte_0=by2;
                    trans.regList[dex+1].byte_1=by3;
                }
            }else if(MB_paraINT16==cfg.paraADC.dataType)
            {
                if(trans.regList.size()>=dex+1)
                {
                    qint16 temp = dataV.dataADC.fData64;
                    quint8 by0,by1;
                    C_endian::int16(cfg.paraADC.boType,temp,by0,by1);
                    trans.regList[dex+0].byte_0=by0;
                    trans.regList[dex+0].byte_1=by1;
                }
            }else if(MB_paraUINT16==cfg.paraADC.dataType)
            {
                if(trans.regList.size()>=dex+1)
                {
                    quint16 temp = dataV.dataADC.fData64;
                    quint8 by0,by1;
                    C_endian::uint16(cfg.paraADC.boType,temp,by0,by1);
                    trans.regList[dex+0].byte_0=by0;
                    trans.regList[dex+0].byte_1=by1;
                }
            }else
            {
                continue;
            }
        }
    }
    emit sig_reuest(trans);  // 定时发送总线请求信号
}

bool C_MB_master_trans::getMonData(QString unitGUID, QString collGUID, QString monInName, MBOutputParaV &data)
{
    unitGUID = unitGUID;
    collGUID = collGUID;
    monInName =monInName;
    data = data;
    return false;
}

// 设置定时事务查询信息
void C_MB_master_trans::setQueryTrans(const confMasterTrans &trans)
{
    this->m_trans = trans;
}

// 启动定时事务查询
void C_MB_master_trans::start()
{
    if(!this->m_timer.isActive())
    {
        this->m_timer.start(this->m_trans.transInfo.timeGap*1000);
    }
}

// 停止定时事务查询
void C_MB_master_trans::stop()
{
    if(this->m_timer.isActive())
    {
        this->m_timer.stop();
    }
}

// 返回 transID值
int C_MB_master_trans::transID()
{
    return this->m_transID;
}

//正常应答
void C_MB_master_trans::replyProc(int transID, quint8 slaveAdr, enumMB_FuncCode fcode, MB_ReplyBody body)
{
    qDebug()<<slaveAdr<<fcode;
    if(transID!=this->m_transID)
    {
        return;
    }
    if(slaveAdr!=this->m_trans.transInfo.trans.slaveAdr)
    {
        return;
    }
    if(fcode!=this->m_trans.transInfo.trans.funcCode)
    {
        return;
    }

    // ... 解析数据
    if(MB_func03==fcode || MB_func04==fcode)
    {
        QList<MBInputParaV>paraList;
        int sum = this->m_trans.listPara.size();
        for(int i=0;i<sum;i++)
        {
            MBInputParaCFG para = this->m_trans.listPara.at(i);
            MBInputParaV data;
            data.name = para.name;
            data.inName = para.inName;
            data.paraType = para.paraType;
            if(mbPara_ADC==data.paraType)
            {
                emumDataType dataType = para.paraADC.dataType;
                enumByteOrderType ordertype = para.paraADC.boType;
                QList<MBregister>reg = body.regList; // 寄存器列表
                int dex =  para.paraADC.paraAdr-this->m_trans.transInfo.trans.beginAdr;  //计算地址->游标
                if(dex<0)
                {
                    continue;
                }
                if(MB_paraREAL32 == dataType)
                {
                    if(body.regList.size()>=dex+2)
                    {
                        float temp = C_endian::float32(ordertype,reg.at(dex+0).byte_0,reg.at(dex+0).byte_1,
                                                                 reg.at(dex+1).byte_0,reg.at(dex+1).byte_1);
                        data.dataADC.fData64 = temp*para.paraADC.factor +para.paraADC.offset;  // 线性变换
                        paraList.append(data);
                    }
                }else if(MB_paraREAL64 == dataType)
                {
                    if(body.regList.size()>=dex+4)
                    {
                        double temp = C_endian::float64(ordertype,reg.at(dex+0).byte_0,reg.at(dex+0).byte_1
                                                                 ,reg.at(dex+1).byte_0,reg.at(dex+1).byte_1
                                                                 ,reg.at(dex+2).byte_0,reg.at(dex+2).byte_1
                                                                 ,reg.at(dex+3).byte_0,reg.at(dex+3).byte_1);
                        data.dataADC.fData64 = temp*para.paraADC.factor +para.paraADC.offset;  // 线性变换
                        paraList.append(data);
                    }
                }else if(MB_paraINT16 == dataType)
                {
                    if(body.regList.size()>=dex+1)
                    {
                        qint16 temp = C_endian::int16(ordertype,reg.at(dex+0).byte_0,reg.at(dex+0).byte_1);
                        data.dataADC.fData64 = temp*para.paraADC.factor +para.paraADC.offset;  // 线性变换
                        paraList.append(data);
                    }
                }else if(MB_paraUINT16 == dataType)
                {
                    if(body.regList.size()>=dex+1)
                    {
                        qint16 temp = C_endian::int16(ordertype,reg.at(dex+0).byte_0,reg.at(dex+0).byte_1);
                        data.dataADC.fData64 = temp*para.paraADC.factor +para.paraADC.offset;  // 线性变换
                        paraList.append(data);
                    }
                }else if(MB_paraINT32 ==dataType)
                {
                    if(body.regList.size()>=dex+2)
                    {
                        qint32 temp = C_endian::int32(ordertype,reg.at(dex+0).byte_0,reg.at(dex+0).byte_1
                                                               ,reg.at(dex+1).byte_0,reg.at(dex+1).byte_1);
                        data.dataADC.fData64 = temp*para.paraADC.factor +para.paraADC.offset;  // 线性变换
                        paraList.append(data);
                    }
                }else if(MB_paraUINT32 == dataType)
                {
                    if(body.regList.size()>=dex+2)
                    {
                        quint32 temp = C_endian::uint32(ordertype,reg.at(dex+0).byte_0,reg.at(dex+0).byte_1
                                                                 ,reg.at(dex+1).byte_0,reg.at(dex+1).byte_1);
                        data.dataADC.fData64 = temp*para.paraADC.factor +para.paraADC.offset;  // 线性变换
                        paraList.append(data);
                    }
                }
            }
        }
        // 测试输出  参数-值
        foreach (MBInputParaV data, paraList)
        {
            qDebug()<<data.name<<QString::number(data.dataADC.fData64,'f',6);
        }

    }else if(MB_func01==fcode || MB_func02==fcode)
    {

    }else if(MB_func10==fcode)
    {
        qDebug()<<this->m_trans.transInfo.name<<QString(" 写保持寄存器 返回: 地址 %1  数目%2").arg(body.wMulR.adr).arg(body.wMulR.sum);
    }
}

//错误应答
void C_MB_master_trans::replyErr(int transID, quint8 slaveAdr, enumMB_FuncCode fcode, RTU_Master_ErrCode errcode)
{
    transID =transID;
    slaveAdr =slaveAdr;
    fcode = fcode;
    errcode = errcode;
}
