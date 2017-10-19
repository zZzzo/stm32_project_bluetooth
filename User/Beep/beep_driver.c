#include "beep_driver.h"


//PC3
void Beep_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1.��GPIOC��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	//��ʼ��PC3��Ϊ�������ģʽ,��ת����Ϊ50MHz
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_9);
}