#include "c_endian.h"
#include<QMessageBox>

C_endian::C_endian()
{

}

bool C_endian::is_big_endian()
{
    return !is_little_endian();
}

void C_endian::bit16(enumByteOrderType boT, C_endian::def_bit16 data, quint8 &by0, quint8 &by1)
{
    bool islocalBig = is_big_endian();
    if(ORDER_BIG==boT)
    {
        if(islocalBig)
        {
            by0 = data.by_0;
            by1 = data.by_1;
        }else
        {
            by0 = data.by_1;
            by1 = data.by_0;
        }
    }else
    {
        if(islocalBig)
        {
            by0 = data.by_1;
            by1 = data.by_0;
        }else
        {
            by0 = data.by_0;
            by1 = data.by_1;
        }
    }
}

void C_endian::bit32(enumByteOrderType boT, C_endian::def_bit32 data, quint8 &by0, quint8 &by1, quint8 &by2, quint8 &by3)
{
    bool islocalBig =is_big_endian();
    if(ORDER_BIG==boT)
    {
        if(islocalBig)
        {
            by0 = data.by_0;
            by1 = data.by_1;
            by2 = data.by_2;
            by3 = data.by_3;
        }else
        {
            by0 = data.by_3;
            by1 = data.by_2;
            by2 = data.by_1;
            by3 = data.by_0;
        }
    }else if(ORDER_BIG_SMALL==boT)
    {
        if(islocalBig)
        {
            by0 = data.by_1;
            by1 = data.by_0;
            by2 = data.by_3;
            by3 = data.by_2;
        }else
        {
            by0 = data.by_2;
            by1 = data.by_3;
            by2 = data.by_0;
            by3 = data.by_1;
        }

    }else if(ORDER_SMALL==boT)
    {
        if(islocalBig)
        {
            by0 = data.by_3;
            by1 = data.by_2;
            by2 = data.by_1;
            by3 = data.by_0;
        }else
        {
            by0 = data.by_0;
            by1 = data.by_1;
            by2 = data.by_2;
            by3 = data.by_3;
        }
    }else if(ORDER_SMALL_BIG==boT)
    {
        if(islocalBig)
        {
            by0 = data.by_2;
            by1 = data.by_3;
            by2 = data.by_0;
            by3 = data.by_1;
        }else
        {
            by0 = data.by_1;
            by1 = data.by_0;
            by2 = data.by_3;
            by3 = data.by_2;
        }
    }
}

C_endian::def_bit16 C_endian::bit16(enumByteOrderType boT, quint8 by0, quint8 by1)
{
    bool islocalBig =is_big_endian();
    def_bit16 temp;
    if(ORDER_BIG==boT)
    {
        if(islocalBig)
        {
            temp.by_0 = by0;
            temp.by_1 = by1;
        }else
        {
            temp.by_0 = by1;
            temp.by_1 = by0;
        }
    }else
    {
        if(islocalBig)
        {
            temp.by_0 = by1;
            temp.by_1 = by0;
        }else
        {
            temp.by_0 = by0;
            temp.by_1 = by1;
        }
    }
    return temp;
}

C_endian::def_bit32 C_endian::bit32(enumByteOrderType boT, quint8 by0, quint8 by1, quint8 by2, quint8 by3)
{
    bool islocalBig =is_big_endian();
    def_bit32 temp;
    if(ORDER_BIG==boT)
    {
        if(islocalBig)
        {
            temp.by_0 = by0;
            temp.by_1 = by1;
            temp.by_2 = by2;
            temp.by_3 = by3;
        }else
        {
            temp.by_0 = by3;
            temp.by_1 = by2;
            temp.by_2 = by1;
            temp.by_3 = by0;
        }
    }else if(ORDER_BIG_SMALL==boT)
    {
        if(islocalBig)
        {
            temp.by_0 = by1;
            temp.by_1 = by0;
            temp.by_2 = by3;
            temp.by_3 = by2;
        }else
        {
            temp.by_0 = by2;
            temp.by_1 = by3;
            temp.by_2 = by0;
            temp.by_3 = by1;
        }
    }
    else if(ORDER_SMALL==boT)
    {
        if(islocalBig)
        {
            temp.by_0 = by3;
            temp.by_1 = by2;
            temp.by_2 = by1;
            temp.by_3 = by0;
        }else
        {
            temp.by_0 = by0;
            temp.by_1 = by1;
            temp.by_2 = by2;
            temp.by_3 = by3;
        }
    }else if(ORDER_SMALL_BIG==boT)
    {
        if(islocalBig)
        {
            temp.by_0 = by2;
            temp.by_1 = by3;
            temp.by_2 = by0;
            temp.by_3 = by1;
        }else
        {
            temp.by_0 = by1;
            temp.by_1 = by0;
            temp.by_2 = by3;
            temp.by_3 = by2;
        }
    }

    return temp;
}

C_endian::def_bit64 C_endian::bit64(enumByteOrderType boT, quint8 by0, quint8 by1, quint8 by2, quint8 by3, quint8 by4, quint8 by5, quint8 by6, quint8 by7)
{
    bool islocalBig =is_big_endian();
    def_bit64 temp;
    if(ORDER_BIG==boT)
    {
        if(islocalBig)
        {
            temp.by_0 = by0;
            temp.by_1 = by1;
            temp.by_2 = by2;
            temp.by_3 = by3;
            temp.by_4 = by4;
            temp.by_5 = by5;
            temp.by_6 = by6;
            temp.by_7 = by7;
        }else
        {
            temp.by_0 = by7;
            temp.by_1 = by6;
            temp.by_2 = by5;
            temp.by_3 = by4;
            temp.by_4 = by3;
            temp.by_5 = by2;
            temp.by_6 = by1;
            temp.by_7 = by0;
        }
    }else if(ORDER_BIG_SMALL==boT)
    {
        if(islocalBig)
        {
            temp.by_0 = by1;
            temp.by_1 = by0;
            temp.by_2 = by3;
            temp.by_3 = by2;
            temp.by_4 = by5;
            temp.by_5 = by4;
            temp.by_6 = by7;
            temp.by_7 = by6;
        }else
        {
            temp.by_0 = by6;
            temp.by_1 = by7;
            temp.by_2 = by4;
            temp.by_3 = by5;
            temp.by_4 = by2;
            temp.by_5 = by3;
            temp.by_6 = by0;
            temp.by_7 = by1;
        }
    }else if(ORDER_SMALL==boT)
    {
        if(islocalBig)
        {
            temp.by_0 = by7;
            temp.by_1 = by6;
            temp.by_2 = by5;
            temp.by_3 = by4;
            temp.by_4 = by3;
            temp.by_5 = by2;
            temp.by_6 = by1;
            temp.by_7 = by0;
        }else
        {
            temp.by_0 = by0;
            temp.by_1 = by1;
            temp.by_2 = by2;
            temp.by_3 = by3;
            temp.by_4 = by4;
            temp.by_5 = by5;
            temp.by_6 = by6;
            temp.by_7 = by7;
        }
    }else if(ORDER_SMALL_BIG==boT)
    {
        if(islocalBig)
        {
            temp.by_0 = by6;
            temp.by_1 = by7;
            temp.by_2 = by4;
            temp.by_3 = by5;
            temp.by_4 = by2;
            temp.by_5 = by3;
            temp.by_6 = by0;
            temp.by_7 = by1;
        }else
        {
            temp.by_0 = by1;
            temp.by_1 = by0;
            temp.by_2 = by3;
            temp.by_3 = by2;
            temp.by_4 = by5;
            temp.by_5 = by4;
            temp.by_6 = by7;
            temp.by_7 = by6;
        }
    }
    return temp;
}

void C_endian::bit64(enumByteOrderType boT, C_endian::def_bit64 data, quint8 &by0, quint8 &by1, quint8 &by2, quint8 &by3, quint8 &by4, quint8 &by5, quint8 &by6, quint8 &by7)
{
    bool islocalBig =is_big_endian();
    if(ORDER_BIG==boT)
    {
        if(islocalBig)
        {
            by0 = data.by_0;
            by1 = data.by_1;
            by2 = data.by_2;
            by3 = data.by_3;
            by4 = data.by_4;
            by5 = data.by_5;
            by6 = data.by_6;
            by7 = data.by_7;
        }else
        {
            by0 = data.by_7;
            by1 = data.by_6;
            by2 = data.by_5;
            by3 = data.by_4;
            by4 = data.by_3;
            by5 = data.by_2;
            by6 = data.by_1;
            by7 = data.by_0;
        }
    }else if(ORDER_BIG_SMALL==boT)
    {
        if(islocalBig)
        {
            by0 = data.by_1;
            by1 = data.by_0;
            by2 = data.by_3;
            by3 = data.by_2;
            by4 = data.by_5;
            by5 = data.by_4;
            by6 = data.by_7;
            by7 = data.by_6;
        }else
        {
            by0 = data.by_6;
            by1 = data.by_7;
            by2 = data.by_4;
            by3 = data.by_5;
            by4 = data.by_2;
            by5 = data.by_3;
            by6 = data.by_0;
            by7 = data.by_1;
        }

    }else if(ORDER_SMALL==boT)
    {
        if(islocalBig)
        {
            by0 = data.by_7;
            by1 = data.by_6;
            by2 = data.by_5;
            by3 = data.by_4;
            by4 = data.by_3;
            by5 = data.by_2;
            by6 = data.by_1;
            by7 = data.by_0;

        }else
        {
            by0 = data.by_0;
            by1 = data.by_1;
            by2 = data.by_2;
            by3 = data.by_3;
            by4 = data.by_4;
            by5 = data.by_5;
            by6 = data.by_6;
            by7 = data.by_7;
        }
    }else if(ORDER_SMALL_BIG==boT)
    {
        if(islocalBig)
        {
            by0 = data.by_6;
            by1 = data.by_7;
            by2 = data.by_4;
            by3 = data.by_5;
            by4 = data.by_2;
            by5 = data.by_3;
            by6 = data.by_0;
            by7 = data.by_1;
        }else
        {
            by0 = data.by_1;
            by1 = data.by_0;
            by2 = data.by_3;
            by3 = data.by_2;
            by4 = data.by_5;
            by5 = data.by_4;
            by6 = data.by_7;
            by7 = data.by_6;
        }
    }
}

// 16比特数据字节序转换
void C_endian::int16(enumByteOrderType boT, qint16 data, quint8 &by0, quint8 &by1)
{
    def_int16 temp;
    temp.value = data;
    C_endian::bit16(boT,temp.bytes,by0,by1);
}

void C_endian::uint16(enumByteOrderType boT, quint16 data, quint8 &by0, quint8 &by1)
{
    def_int16 temp;
    temp.value = data;
    C_endian::bit16(boT,temp.bytes,by0,by1);
}

void C_endian::int32(enumByteOrderType boT, qint32 data, quint8 &by0, quint8 &by1, quint8 &by2, quint8 &by3)
{
    def_int32 temp;
    temp.value = data;
    C_endian::bit32(boT,temp.bytes,by0,by1,by2,by3);
}

void C_endian::uint32(enumByteOrderType boT, quint32 data, quint8 &by0, quint8 &by1, quint8 &by2, quint8 &by3)
{
    def_uint32 temp;
    temp.value = data;
    C_endian::bit32(boT,temp.bytes,by0,by1,by2,by3);
}

void C_endian::float32(enumByteOrderType boT, float data, quint8 &by0, quint8 &by1, quint8 &by2, quint8 &by3)
{
    def_float temp;
    temp.value = data;
    C_endian::bit32(boT,temp.bytes,by0,by1,by2,by3);
}

void C_endian::float64(enumByteOrderType boT, double data, quint8 &by0, quint8 &by1, quint8 &by2, quint8 &by3, quint8 &by4, quint8 &by5, quint8 &by6, quint8 &by7)
{
    def_double temp;
    temp.value = data;
    C_endian::bit64(boT,temp.bytes,by0,by1,by2,by3,by4,by5,by6,by7);
}

qint16 C_endian::int16(enumByteOrderType boT, quint8 by0, quint8 by1)
{
    def_int16 temp;
    temp.bytes = C_endian::bit16(boT,by0,by1);

    return temp.value;
}

quint16 C_endian::uint16(enumByteOrderType boT, quint8 by0, quint8 by1)
{
    def_uint16 temp;
    temp.bytes = C_endian::bit16(boT,by0,by1);

    return temp.value;
}

qint32 C_endian::int32(enumByteOrderType boT, quint8 by0, quint8 by1, quint8 by2, quint8 by3)
{
    def_int32 temp;
    temp.bytes = C_endian::bit32(boT,by0,by1,by2,by3);

    return temp.value;
}

quint32 C_endian::uint32(enumByteOrderType boT, quint8 by0, quint8 by1, quint8 by2, quint8 by3)
{
    def_uint32 temp;
    temp.bytes = C_endian::bit32(boT,by0,by1,by2,by3);

    return temp.value;
}

float C_endian::float32(enumByteOrderType boT, quint8 by0, quint8 by1, quint8 by2, quint8 by3)
{
    def_float temp;
    temp.bytes = C_endian::bit32(boT,by0,by1,by2,by3);

    return temp.value;
}

double C_endian::float64(enumByteOrderType boT, quint8 by0, quint8 by1, quint8 by2, quint8 by3, quint8 by4, quint8 by5, quint8 by6, quint8 by7)
{
    def_double temp;
    temp.bytes = C_endian::bit64(boT,by0,by1,by2,by3,by4,by5,by6,by7);

    return temp.value;
}

bool C_endian::is_little_endian()
{
    uint32_t _endian_x_ = 1;
    return (reinterpret_cast<const uint8_t*>(& _endian_x_))[0];
}
