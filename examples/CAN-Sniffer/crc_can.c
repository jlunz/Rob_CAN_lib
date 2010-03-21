// CAN-CRC x15 + x14 + x10 + x8 + x7 + x4 + x3 + 1

#include <avr/io.h>


uint16_t crc_can(uint16_t crc, uint8_t data)
{
  uint8_t i;
  
  for(i = 0; i < 8; i++)
  {
    if((crc ^ data) & 1)
    { 
      crc = (uint16_t)((crc >> 1) ^ 0xC599); 
    }
    else
    {
      crc >>= 1;
    }
    data >>= 1;
  }
  return (crc);
}
