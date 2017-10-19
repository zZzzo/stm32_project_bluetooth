#include "usart_driver.h"



void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	
	//配置Tx PA9 复用推免输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置Rx PA10 浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART1配置
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStruct);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
	//初始化 把SR寄存器 TC位清零 (发送完成标志位)
	//否则接收数据时，第一位会丢失
//	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART1->SR &= ~(1<<6);
	//启用USART1
	USART_Cmd(USART1, ENABLE);
}

//1、NVIC嵌套向量中断控制器配置
//2、要使用中断的外设也要配置  中断使能
//3、中断服务程序
void UASRT_NVIC_Configuration(void)
{
#if 0
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#else
	//另一种设置NVIC方法：
	NVIC_SetPriority(USART1_IRQn,3);
	NVIC_EnableIRQ(USART1_IRQn);
#endif
	
}



#if 1
//stdout
/****************************
**重写fputc(int ch,FILF *f),
**即重定向标准输出
*****************************/
int fputc(int ch, FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);

		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}
//stdin
/****************************
**重写fgetc(FILF *f),
**即重定向标准输入
*****************************/
int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(USART1);
}
#endif

/****************************
**写 字节
*****************************/
char usart1_putc(char ch)
{
	USART_SendData(USART1,(uint16_t)ch);

	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
	
	return(ch);
}

/****************************
**写 字符串---1
*****************************/
int usart1_putstr( char *buff,int len)
{
	int i = 0;
	for(; i <len; i++)
		usart1_putc(buff[i]);
	return len;
}
/****************************
**写 字符串---
*****************************/
int usart1_print(unsigned char *buff)
{
	int i =0;
	while(*buff != '\0')
	{
		i++;
		usart1_putc(*buff++);
	}
	return i;
}

/****************************
**读  字节
*****************************/
char usart1_getc(void)
{
	char temp = 0;
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) != SET);
	temp = USART_ReceiveData(USART1);
	return temp;
}
/****************************
**读 字符串
*****************************/
int usart1_getstr(unsigned char *buff,int len)
{
	int i = 0;
	for(; i<len; i++)
	{
		buff[i] = usart1_getc();
	}
	
	return len;
}

void USART_InitConfig(void)
{
	USART1_Config();
	UASRT_NVIC_Configuration();
}







