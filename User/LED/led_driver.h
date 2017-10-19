#ifndef _LED_DRIVER_H_
#define _LED_DRIVER_H_

#include "stm32f10x.h"

#define ON		0
#define OFF		1

typedef enum{
	LED_RED = 0,
	LED_GREEN,
	LED_BLUE,
}__LED_TYPE;


#if 1
void Led_InitConfig(void);
void Led_Ctl(__LED_TYPE type,uint8_t statue);

#else
void LED_GPIO_InitConfig(void);
void LED_All_ON(void);
void LED_All_OFF(void);
void Red_LED(u16 pwm);
void Green_LED(u16 pwm);
void Blue_LED(u16 pwm);
					
					
void BreadtheLight(void);

#endif
#endif

