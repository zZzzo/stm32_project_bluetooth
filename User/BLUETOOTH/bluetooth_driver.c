#include "bluetooth_driver.h"
#include "SysTick.h"
#include "usart_driver.h"

#define BRTS_Pin	GPIO_Pin_13

//唤醒
#define BRTS_ON		GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define BRTS_OFF	GPIO_SetBits(GPIOB,GPIO_Pin_13)

void BT_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//配置Tx PB10 复用推免输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//配置Rx PB11 浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//USART3配置
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3, &USART_InitStruct);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	//初始化 把SR寄存器 TC位清零 (发送完成标志位)
	//否则接收数据时，第一位会丢失
	USART_ClearFlag(USART3, USART_FLAG_TC);
//	USART3->SR &= ~(1<<6);
	//启用USART3
	USART_Cmd(USART3, ENABLE);
	
	//EN
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	//流控
	GPIO_InitStruct.GPIO_Pin = BRTS_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//模块使能
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	//流控关闭
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}

//1、NVIC嵌套向量中断控制器配置
//2、要使用中断的外设也要配置  中断使能
//3、中断服务程序
void UASRT3_NVIC_Configuration(void)
{

	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void BT_Init(void)
{	
	BT_InitConfig();
	UASRT3_NVIC_Configuration();
}	


/****************************
**写 字节
*****************************/
char usart3_putc(char ch)
{
	USART_SendData(USART3,(uint16_t)ch);

	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);
	
	return(ch);
}
/****************************
**写 字符串
*****************************/
int usart3_putstr( char *buff,int len)
{
	int i = len;
	for(; i>=0; i--)
	{
		usart3_putc(buff[i]);
	//	delay_ms(25);
	//	printf("buf[%d] = %x\r\n",i,buff[i]);
	}
//	printf("----------------------\r\n");
	return len;
}

void Bluetooth_SendMsg2(char *buf,unsigned int length)
{
	unsigned int i;
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	delay_ms(10);								
	for(i = 0; i < length; i++)
	   usart3_putc(buf[i]);
	delay_ms(10);	
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}

void Bluetooth_SendMsg(char *buf)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_13);
	delay_ms(10);								
	usart3_putstr(buf,7);
	delay_ms(10);	
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}
/*BCC校验*/
u8 CheakBcc(char *buf)
{
	u8 i;
	u8 ret = 0;
	for(i=1; i<5; i++)
		{
			ret ^= buf[i];
		}
	return ret;
}
//温度打包
void TempMsg(char *buf,u8 temp,u8 tem)
{
	buf[4] = 0x04;
	buf[3] = 0x60;
	buf[2] = temp;
	buf[1] = tem;
	buf[0] = CheakBcc(buf);
}
//湿度打包
void HMsg(char *buf,u8 humidity)
{
//	printf("HMSG h = %d\r\n",humidity);
	buf[4] = 0x04;
	buf[3] = 0x61;
	buf[2] = humidity;
	buf[1] = 0;
	buf[0] = CheakBcc(buf);
}
//光强打包
void LxMsg(char *buf,u16 lx)
{
	buf[4] = 0x04;
	buf[3] = 0x22;
	buf[2] = lx>>8;
	buf[1] = lx & 0x00ff;
	buf[0] = CheakBcc(buf);
}

	


