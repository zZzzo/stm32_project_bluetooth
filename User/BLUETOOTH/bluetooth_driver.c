#include "bluetooth_driver.h"
#include "SysTick.h"
#include "usart_driver.h"

#define BRTS_Pin	GPIO_Pin_13

//����
#define BRTS_ON		GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define BRTS_OFF	GPIO_SetBits(GPIOB,GPIO_Pin_13)

void BT_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//����Tx PB10 �����������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//����Rx PB11 ��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//USART3����
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART3, &USART_InitStruct);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	//��ʼ�� ��SR�Ĵ��� TCλ���� (������ɱ�־λ)
	//�����������ʱ����һλ�ᶪʧ
	USART_ClearFlag(USART3, USART_FLAG_TC);
//	USART3->SR &= ~(1<<6);
	//����USART3
	USART_Cmd(USART3, ENABLE);
	
	//EN
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	//����
	GPIO_InitStruct.GPIO_Pin = BRTS_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//ģ��ʹ��
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	//���عر�
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}

//1��NVICǶ�������жϿ���������
//2��Ҫʹ���жϵ�����ҲҪ����  �ж�ʹ��
//3���жϷ������
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
**д �ֽ�
*****************************/
char usart3_putc(char ch)
{
	USART_SendData(USART3,(uint16_t)ch);

	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);
	
	return(ch);
}
/****************************
**д �ַ���
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
/*BCCУ��*/
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
//�¶ȴ��
void TempMsg(char *buf,u8 temp,u8 tem)
{
	buf[4] = 0x04;
	buf[3] = 0x60;
	buf[2] = temp;
	buf[1] = tem;
	buf[0] = CheakBcc(buf);
}
//ʪ�ȴ��
void HMsg(char *buf,u8 humidity)
{
//	printf("HMSG h = %d\r\n",humidity);
	buf[4] = 0x04;
	buf[3] = 0x61;
	buf[2] = humidity;
	buf[1] = 0;
	buf[0] = CheakBcc(buf);
}
//��ǿ���
void LxMsg(char *buf,u16 lx)
{
	buf[4] = 0x04;
	buf[3] = 0x22;
	buf[2] = lx>>8;
	buf[1] = lx & 0x00ff;
	buf[0] = CheakBcc(buf);
}

	


