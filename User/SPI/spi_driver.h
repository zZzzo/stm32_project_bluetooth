#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#include "stm32f10x.h"


#define SPI1_BUFF_MAX	512

/**********************************************************/
extern u8 SPI_Send_Data;
extern u8 SPI_Read_Data;
/*********************************************************/

void SPI_InitConfig(void);
u8 SPI_RW(u8 dat);
void SPI_Device_Init(void);
void SPI_WriterByte(u8 dat);


#endif
