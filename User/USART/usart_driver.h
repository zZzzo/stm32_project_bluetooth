#ifndef _USART_DRIVER_H_
#define _USART_DRIVER_H_

#include "stm32f10x.h"
#include "ST_string.h"
#include <stdio.h>

/*****************************************************/
#define USART1_DEBUG
/*****************************************************/



void USART1_Config(void);
char usart1_putc(char ch);
int usart1_putstr( char *buff,int len);
int usart1_print(unsigned char *buff);
char usart1_getc(void);
void USART_InitConfig(void);

#endif

