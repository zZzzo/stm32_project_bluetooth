#include "SysTick.h"

static u8  fac_us=0;//us��ʱ������
static u16 fac_ms=0;//ms��ʱ������
  
void SysTick_init(u8 SYSCLK)
{
	SysTick->CTRL &= 0xfffffffb;	//bit2��0��ѡ��HCLK/8
	
	fac_us = SYSCLK/8;				//ѡ����HCLK/8 ����Ҫ/8
	fac_ms = (u16)fac_us*1000;
}

void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD = nus*fac_us;			//��ʱ10us�Ļ����� 10*9 = 90,װ�ص�LOAD�Ĵ����С�û���Զ�װ��
	SysTick->VAL = 0x00;				//��������0����Ϊcurrent�ֶα��ֶ�����ʱ��LOAD���Զ�װ�ص�VAL�С�
	SysTick->CTRL = 0x01;				//����ʹ�쳣��Ч����������������0ʱ�������쳣֪ͨ��
	
	do
	{
		temp = SysTick->CTRL;			//ʱ�䵽��֮�󣬸�λ����Ӳ����1��������ѯ���Զ���0. 
	}while((temp&0x01) && !(temp&(1<<16)));	//��ѯ������(temp&0x01)���ⶨʱ�����رյ�������ѭ����
	SysTick->CTRL = 0x00;					//�رռ�����
	SysTick->VAL = 0x00;					//���VAL
}

//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;
	SysTick->LOAD = nms*fac_ms;
	SysTick->VAL = 0x00;
	SysTick->CTRL = 0x01;
	
	do
	{
		temp = SysTick->CTRL;
	}while(temp & 0x01 && !(temp & (1<<16)));
	
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
}   
