#include "tim_driver.h"


static void TIM2_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	//打开TIM2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7199;// 预分配系数为7199,即计数频率为72MHz/(7199+1)=10Khz
	TIM_TimeBaseInitStruct.TIM_Period = 9999;//定时1s的计数	
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);//开启TIM2中断
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//情况事件更新标志位
	
	TIM_Cmd(TIM2, ENABLE);//使能TIM2
}


static void TIM_NVIC_InitConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	//设置优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//选择中断通道
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;//抢占式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;//响应式优先级设置
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能
	NVIC_Init(&NVIC_InitStruct);
}


void TIM_InitConfig(void)
{
	TIM2_Config();
	TIM_NVIC_InitConfig();
}
