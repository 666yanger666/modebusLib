#ifndef C_ENDIAN_H
#define C_ENDIAN_H
#include "../../../WORK/HLMonitorGit/include/include.h"

// 大端、小端字节序移植封装  类定义

// 判断本地机器 大端、小端模式
class C_endian
{

public:
    C_endian();
public:
    struct def_bit16
    {
        quint8  by_0;
        quint8  by_1;
    };

    struct def_bit32
    {
        quint8 by_0;
        quint8 by_1;
        quint8 by_2;
        quint8 by_3;
    };

    struct def_bit64
    {
        quint8 by_0;
        quint8 by_1;
        quint8 by_2;
        quint8 by_3;
        quint8 by_4;
        quint8 by_5;
        quint8 by_6;
        quint8 by_7;
    };
    union def_uint16
    {
        quint16 value;
        def_bit16 bytes;
    };

    union def_int16
    {
        qint16 value;
        def_bit16 bytes;
    };

    union def_uint32
    {
        quint32 value;
        def_bit32 bytes;
    };

    union def_int32
    {
        qint32 value;
        def_bit32 bytes;
    };

    union def_float
    {
        float value;
        def_bit32 bytes;
    };

    union def_double
    {
        double value;
        def_bit64 bytes;
    };
private:
      static bool is_little_endian();
      static bool is_big_endian();

      static void bit16(enumByteOrderType boT,def_bit16 data,quint8 &by0,quint8 &by1);
      static void bit32(enumByteOrderType boT,def_bit32 data,quint8 &by0,quint8 &by1,quint8 &by2,quint8 &by3);
      static void bit64(enumByteOrderType boT,def_bit64 data,quint8 &by0,quint8 &by1,quint8 &by2,quint8 &by3,quint8 &by4,quint8 &by5,quint8 &by6,quint8 &by7);

      static def_bit16 bit16(enumByteOrderType boT,quint8 by0,quint8 by1);
      static def_bit32 bit32(enumByteOrderType boT,quint8 by0,quint8 by1,quint8 by2,quint8 by3);
      static def_bit64 bit64(enumByteOrderType boT,quint8 by0,quint8 by1,quint8 by2,quint8 by3,quint8 by4,quint8 by5,quint8 by6,quint8 by7);
public:
    static void int16(enumByteOrderType boT,qint16 data,quint8 &by0,quint8 &by1);
    static void uint16(enumByteOrderType boT,quint16 data,quint8 &by0,quint8 &by1);
    static void int32(enumByteOrderType boT,qint32 data,quint8 &by0,quint8 &by1,quint8 &by2,quint8 &by3);
    static void uint32(enumByteOrderType boT,quint32 data,quint8 &by0,quint8 &by1,quint8 &by2,quint8 &by3);
    static void float32(enumByteOrderType boT,float data,quint8 &by0,quint8 &by1,quint8 &by2,quint8 &by3);
    static void float64(enumByteOrderType boT,double data,quint8 &by0,quint8 &by1,quint8 &by2,quint8 &by3,quint8 &by4,quint8 &by5,quint8 &by6,quint8 &by7);

    static qint16 int16(enumByteOrderType boT,quint8 by0,quint8 by1);
    static quint16 uint16(enumByteOrderType boT,quint8 by0,quint8 by1);
    static qint32 int32(enumByteOrderType boT,quint8 by0,quint8 by1,quint8 by2,quint8 by3);
    static quint32 uint32(enumByteOrderType boT,quint8 by0,quint8 by1,quint8 by2,quint8 by3);
    static float float32(enumByteOrderType boT,quint8 by0,quint8 by1,quint8 by2,quint8 by3);
    static double float64(enumByteOrderType boT,quint8 by0,quint8 by1,quint8 by2,quint8 by3,quint8 by4,quint8 by5,quint8 by6,quint8 by7);

};

#endif // C_ENDIAN_H
