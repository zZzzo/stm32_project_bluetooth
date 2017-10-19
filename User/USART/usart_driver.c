#include "usart_driver.h"



void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	
	//����Tx PA9 �����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//����Rx PA10 ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//USART1����
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStruct);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	
	//��ʼ�� ��SR�Ĵ��� TCλ���� (������ɱ�־λ)
	//�����������ʱ����һλ�ᶪʧ
//	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART1->SR &= ~(1<<6);
	//����USART1
	USART_Cmd(USART1, ENABLE);
}

//1��NVICǶ�������жϿ���������
//2��Ҫʹ���жϵ�����ҲҪ����  �ж�ʹ��
//3���жϷ������
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
	//��һ������NVIC������
	NVIC_SetPriority(USART1_IRQn,3);
	NVIC_EnableIRQ(USART1_IRQn);
#endif
	
}



#if 1
//stdout
/****************************
**��дfputc(int ch,FILF *f),
**���ض����׼���
*****************************/
int fputc(int ch, FILE *f)
{
		USART_SendData(USART1, (uint8_t) ch);

		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}
//stdin
/****************************
**��дfgetc(FILF *f),
**���ض����׼����
*****************************/
int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(USART1);
}
#endif

/****************************
**д �ֽ�
*****************************/
char usart1_putc(char ch)
{
	USART_SendData(USART1,(uint16_t)ch);

	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);
	
	return(ch);
}

/****************************
**д �ַ���---1
*****************************/
int usart1_putstr( char *buff,int len)
{
	int i = 0;
	for(; i <len; i++)
		usart1_putc(buff[i]);
	return len;
}
/****************************
**д �ַ���---
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
**��  �ֽ�
*****************************/
char usart1_getc(void)
{
	char temp = 0;
	
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) != SET);
	temp = USART_ReceiveData(USART1);
	return temp;
}
/****************************
**�� �ַ���
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







