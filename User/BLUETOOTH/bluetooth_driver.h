#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "stm32f10x.h"

void BT_Init(void);
void UASRT3_NVIC_Configuration(void);
void BT_InitConfig(void);
char usart3_putc(char ch);
int usart1_putstr(char *buff,int len);
int uasrt3_print(unsigned char *buff);
void Bluetooth_SendMsg(char *buf);
u8 CheakBcc(char *buf);
void TempMsg(char *buf,u8 temp,u8 tem);
void HMsg(char *buf,u8 humidity);
void LxMsg(char *buf,u16 lx);
void Bluetooth_SendMsg2(char *buf,unsigned int length);

#endif