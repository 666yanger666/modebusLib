#ifndef C_CRC_H
#define C_CRC_H
#include <QObject>

class C_CRC
{
public:
    C_CRC();

public:
    static uint16_t crc16(uint8_t *buffer, uint16_t buffer_length);
};

#endif // C_CRC_H
