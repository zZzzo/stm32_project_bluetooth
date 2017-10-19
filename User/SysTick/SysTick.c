#include "SysTick.h"

static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数
  
void SysTick_init(u8 SYSCLK)
{
	SysTick->CTRL &= 0xfffffffb;	//bit2清0，选择HCLK/8
	
	fac_us = SYSCLK/8;				//选择了HCLK/8 所以要/8
	fac_ms = (u16)fac_us*1000;
}

void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD = nus*fac_us;			//延时10us的话就是 10*9 = 90,装载到LOAD寄存器中。没有自动装载
	SysTick->VAL = 0x00;				//计数器清0，因为current字段被手动清零时，LOAD将自动装载到VAL中。
	SysTick->CTRL = 0x01;				//配置使异常生效，当计数器倒数到0时将发出异常通知。
	
	do
	{
		temp = SysTick->CTRL;			//时间到了之后，该位将被硬件置1，但被查询后自动清0. 
	}while((temp&0x01) && !(temp&(1<<16)));	//查询，这里(temp&0x01)避免定时器被关闭导致无限循环。
	SysTick->CTRL = 0x00;					//关闭计数器
	SysTick->VAL = 0x00;					//清空VAL
}

//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864
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
	
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
}   
