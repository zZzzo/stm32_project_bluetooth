#ifndef _DHT11_DRIVER_H_
#define _DHT11_DRIVER_H_

#include "stm32f10x.h"



void DHT11_InitConfig(void);
void DHT11_IO_IN(void);
void DHT11_IO_OUT(void);
u8 DHT11_Start(void);
u8 ReadBit(void);
u8 ReadByte(void);
u8 ReadDat(u8 *temp, u8 *tem, u8 *humidity, u8 *hum);


#endif

