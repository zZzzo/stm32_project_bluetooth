#include "tim_driver.h"


static void TIM2_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//��TIM2��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;// Ԥ����ϵ��Ϊ7199,������Ƶ��Ϊ72MHz/(7199+1)=10Khz
	TIM_TimeBaseInitStruct.TIM_Period = 9999;//��ʱ1s�ļ���	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);//����TIM2�ж�
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//����¼����±�־λ
	
	TIM_Cmd(TIM2, ENABLE);//ʹ��TIM2
}


static void TIM_NVIC_InitConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	//�������ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//ѡ���ж�ͨ��
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;//��ռʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;//��Ӧʽ���ȼ�����
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	NVIC_Init(&NVIC_InitStruct);
}


void TIM_InitConfig(void)
{
	TIM2_Config();
	TIM_NVIC_InitConfig();
}
