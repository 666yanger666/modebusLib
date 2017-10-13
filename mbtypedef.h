#ifndef MBTYPEDEF_H
#define MBTYPEDEF_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


// RTU 主机模式 状态机 状态定义
enum RTU_MasterSta
{
    RTUmaster_IDEL =0,     // 空闲
    RTUmaster_CONVDELAY=1, // 转换延迟
    RTUmaster_WAITREPLY,   // 等待应答
    RTUmaster_PROCREPLY,   // 处理应答
    RTUmaster_ERRPROC      // 出错处理
};

//  RTU 主机模式 错误码定义
enum RTU_Master_ErrCode
{
    RTUmasterErr_TIMEOUT =0, // 响应超时
    RTUmasterErr_REPLY01 =1, // 非法功能码 配合功能码
    RTUmasterErr_REPLY02,    // 非法数据地址
    RTUmasterErr_REPLY03,    // 非法数据值
    RTUmasterErr_REPLY04,    // 从站设备故障
    RTUmaster_UNKOWN         // 未知定义
};

// 从机模式 状态机 状态定义
enum RTU_SlaveSta
{
   slave_IDEL =0,       // 空闲
   slave_CHECKREQ,      // 检查请求
   slave_PROCREQ,       // 处理请求
   slave_NORMALREPLY,   // 正常应答
   slave_ERRREPLY       // 错误应答
};

// MODEBUS节点类型
enum enumMBNodeType
{
    MB_RTU_MASTER = 0,  //RTU 主机
    MB_RTU_SLAVE = 1,   //RTU 从机
    MB_TCP_MASTER,      //TCP 主机
    MB_TCP_SLAVE,       //TCP 从机
    MB_RTU_MASTER_GATE, //RTU 主机 网关
    MB_RTU_SLAVE_GATE,  //RTU 从机 网关
};

// modbus 请求读数据功能枚举
enum enumMB_FuncCode
{
    MB_func01 = 0X01,  // 读线圈 (开出)
    MB_func02 = 0X02,  // 读离散量 (开入)
    MB_func03 = 0X03,  // 读保持寄存器
    MB_func04 = 0X04   // 读输入寄存器
};

struct MB_ReplyBody
{
    QByteArray swt;    // 开关量
    QList<quint16>reg; // 寄存器
};

struct MBRequestTrans
{
    quint8 slaveAdr;          // 对方从站地址
    enumMB_FuncCode funcCode; // 功能码
    quint16 beginAdr;         // 起始地址
    quint16 paraSum;          // 请求参数数目
};

struct MBRequestTransEx
{
    MBRequestTrans trans;
    int transID;
};

// 定义请求事务结构配置信息
struct MBRequestTransInfo
{
    MBRequestTrans trans;     // 请求事务
    qint32  timeGap;          // 定时间隔(单位/秒)
};

// MB 参数类型
enum enumMBParaType
{
    mbPara_SWT=0, //开关量
    mbPara_ADC=1  //模拟量
};

// 数据有符号、无符号
enum enumMBParaSigned
{
    para_UNSIGNED=0, // 无符号
    para_SIGNED=1    // 有符号
};

//  MB外部输入参数信息结构
struct MBInputParaCFG
{
    quint16 paraAdr;            //地址编号
    QString name;               //名称
    QString inName;             //内部名称
    double factor;              //变比系数
    double offset;              //偏置
    enumMBParaSigned paraSign;  //参数符号
    QString remark;             //备注
};

// 串口配置信息结构
struct serialPortInfo
{
    QString comName;  // COM口名称
    qint32 baudRate;  // 波特率
    enum QSerialPort::DataBits  dataBits;       // 数据位  5，6，7，8
    enum QSerialPort::StopBits stopBits;        // 停止位  1, 1.5, 2
    enum QSerialPort::Parity   parity;          // 校验位  无，奇，偶，标记，空格
    enum QSerialPort::FlowControl flowControl;  // 流控
};

// rtu master node 节点基本信息
struct RTUMasterNodeInfo
{
    serialPortInfo serialInfo;  // 串口配置信息
    QString remark;
};

// rtu master trans 事务配置信息结构
struct confRTUMasterTrans
{
    MBRequestTransInfo transInfo;  // 事务基本信息
    QList<MBInputParaCFG>listPara; // 参数配置列表
};

// mb-rtu-master-Node  节点配置机构
struct confRTUMasterNode
{
   RTUMasterNodeInfo nodeInfo;          // 节点基本信息
   QList<confRTUMasterTrans>listTrans;  // 事务配置列表
};

// MB_RTU_SLAVE 配置信息
struct confRTUSlaveInfo
{

};

// MB_RTU_SLAVE 配置信息
struct confTCPMasterInfo
{

};

// MB_RTU_SLAVE 配置信息
struct confTCPSlaveInfo
{

};

// MB_RTU_SLAVE 配置信息
struct confRTUMasterGateInfo
{

};

// MB_RTU_SLAVE 配置信息
struct confRTUSlaveGateInfo
{

};

// MB通讯节点配置信息
union confMBNodeInfo
{
    QString Name ;
    enum enumMBNodeType nodeType;
//  confRTUMasterInfo rtuMaster;
    confRTUSlaveInfo  rtuSlave;
    confTCPMasterInfo tcpMaster;
    confTCPSlaveInfo  tcpSlave;
    confRTUMasterGateInfo rtuMasterGate;
    confRTUSlaveGateInfo  rtuSlaveGate;
};

// Modmus通讯接口数据单元
struct confMBNodeTT
{
    enumMBNodeType nodeType;
};

#endif // MBTYPEDEF_H
