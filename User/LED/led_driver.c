#include "led_driver.h"
#include "SysTick.h"

#if 1
/****�򵥵���ɫLED****/
/***PWM���Ƶ�LED��****/
/**********************************************
** 3ɫLED��ʼ��
**********************************************/
void Led_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//APB2ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//��ʼ���ṹ��
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	//��ʼ��-�ر�LED��
	GPIO_SetBits(GPIOC, GPIO_Pin_6);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);
}
/**********************************************
** 3ɫLED����
**********************************************/
void Led_Ctl(__LED_TYPE type,uint8_t statue)
{
	switch((uint8_t)type)
	{
		case LED_RED:
			if(statue)
				GPIO_SetBits(GPIOC, GPIO_Pin_8);
			else	
				GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		break;
		case LED_GREEN:
			if(statue)
				GPIO_SetBits(GPIOC, GPIO_Pin_7);
			else
				GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		break;
		case LED_BLUE:
			if(statue)
				GPIO_SetBits(GPIOC, GPIO_Pin_6);
			else
				GPIO_ResetBits(GPIOC, GPIO_Pin_6);
		break;
	}
}

#else

/***PWM���Ƶ�LED��****/

#if 1
uint16_t LED_indexWave[10] = {0,30,50,70,100,140,180,200,240,255};
#else
uint16_t LED_indexWave[10] = {255,240,200,180,140,100,70,50,30,0};
#endif
/*********************************************************************
******************************LED�ƶ���*******************************
**LED_R-----PC6
**LED_G-----PC7
**LED_B-----PC8
**********************************************************************/

/**********************************************************
**�������ܣ�LED�˿ڳ�ʼ��
***********************************************************/
void LED_GPIO_InitConfig(void)
{
	//����һ��GPIO_InitTypeDef���͵Ľṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
// 	u16 CCR1_Val = 32;  // PWM�źŵ�ƽ����ֵ 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); //Timer1��ȫ��ӳ��  TIM3_CH1->PC6 CH2->PC7 CH1->PC8
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	/* ----------------------------------------------------------------------- 
    TIM3 Channe2 duty cycle = (TIM3_CCR2/ TIM3_ARR+1)* 100% = PWMռ�ձ�
  ----------------------------------------------------------------------- */
	// Time base configuration 	 
	TIM_TimeBaseStructure.TIM_Period = 255;       //����ʱ����0������255����Ϊ256�Σ�Ϊһ����ʱ����
	TIM_TimeBaseStructure.TIM_Prescaler = 1999;	    //����Ԥ��Ƶ��2000Ԥ��Ƶ����Ϊ36KHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0 ;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	// PWM1 Mode configuration: Channe1 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
	TIM_OCInitStructure.TIM_Pulse = 0;	   //��������ֵ�������������������ֵʱ����ƽ��������
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//����ʱ������ֵС��CCR1_Valʱ,�����Ч��ƽ
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��2
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��3
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM3, ENABLE);
	// TIM4 enable counter 
	TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3

//	TIM3->BDTR|=0x8000;  //ʹ�ܶ�ʱ��3���
	
	LED_All_OFF();
}


/**********************************************************
**�������ܣ�ȫ������LED
***********************************************************/
void LED_All_ON(void)
{
	Red_LED(255);
	Green_LED(255);
	Blue_LED(255);
}

/**********************************************************
**�������ܣ�ȫ��Ϩ��LED
***********************************************************/
void LED_All_OFF(void)
{
	Red_LED(0);
	Green_LED(0);
	Blue_LED(0);
}



static void PWM_Channel1_Out(u16 pwm)
{
	TIM3->CCR1 =  pwm;
}


static void PWM_Channel2_Out(u16 pwm)
{
	TIM3->CCR2 =  pwm;
}

static void PWM_Channel3_Out(u16 pwm)
{
	TIM3->CCR3 =  pwm;
}


void Red_LED(u16 pwm)
{
	PWM_Channel3_Out(pwm);
}

void Green_LED(u16 pwm)
{
	PWM_Channel2_Out(pwm);
}

void Blue_LED(u16 pwm)
{
	PWM_Channel1_Out(pwm);
}

void BreadtheLight(void)
{
	uint8_t i = 0;
	
	for(i = 0;i < 10;i++)
	{
		Green_LED(LED_indexWave[i]);
		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
	}
}

#endif
