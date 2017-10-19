#include "stm32f10x.h"
#include "sys_rcc.h"
#include "SysTick.h"
#include "usart_driver.h"
#include "spi_driver.h"
#include "ST_String.h"
#include "oled_driver.h"
#include "adc_driver.h"
#include "dht11_driver.h"
#include "led_driver.h"
#include "bluetooth_driver.h"
#include "beep_driver.h"
#include "tim_driver.h"

char buf[8];
u8 temp = 0;
u8 humidity = 0;
u8 tem = 0;
u8 hum = 0;
int lx;

#if 0
char usart3_buf[9];
int length = 0;
char red,green,blue = 0;
#endif

int main(void)
{	
	u8 i;
	
	SysTick_init(72);
	USART_InitConfig();
	SPI_InitConfig();
	LCD_Init();
	ADC1_Init();
	DHT11_InitConfig();
	Led_InitConfig();
	BT_Init();
	TIM_InitConfig();
	
	delay_ms(30);
	buf[7] = 0xff;
	buf[6] = 0xff;
	buf[5] = 0x00;
	while(1)
	{
		//采集数据
		ReadDat(&temp,&tem,&humidity,&hum);
		lx =4095 - Get_Adc();

		//显示温度
		OLED_ShowCHinese(0,0,6);	//实
		OLED_ShowCHinese(16,0,7);	//时
		OLED_ShowCHinese(32,0,0);	//温
		OLED_ShowCHinese(48,0,1);	//度
		LCD_P8x16Str(62,0,":");
		OLED_ShowNum(74,0,temp,2,16);
		LCD_P8x16Str(89,0,".");
		OLED_ShowNum(93,0,tem,1,16);
		//显示湿度
		OLED_ShowCHinese(0,2,6);	//实
		OLED_ShowCHinese(16,2,7);	//时
		OLED_ShowCHinese(32,2,2);	//湿
		OLED_ShowCHinese(48,2,1);	//度
		LCD_P8x16Str(62,2,":");
		OLED_ShowNum(74,2,humidity,2,16);
		LCD_P8x16Str(91,2,"%");
		//显示光强
		OLED_ShowCHinese(0,4,3);	//光
		OLED_ShowCHinese(16,4,4);	//照
		OLED_ShowCHinese(32,4,5);	//强
		OLED_ShowCHinese(48,4,1);	//度
		LCD_P8x16Str(62,4,":");
		OLED_ShowNum(74,4,lx,4,16);	
		
		
		
/******		光强报警			********/
		if(lx < 500)
			Led_Ctl(LED_RED,ON);
		else
			Led_Ctl(LED_RED,OFF);
		delay_ms(200);		
	//	printf("blue == %c\r\n",blue);
	}
}
int statue = 0;


/****			蓝牙发送数据		***********/
//
void TIM2_IRQHandler(void)
{
	  static uint8_t i;  
	  
	//判断是否为TIM_IT_Update事件产生
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		//清空中断挂起位 TIM_IT_Update
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		if(statue%3 == 0)
		{
			//发温度
			TempMsg(buf,temp,tem);
			Bluetooth_SendMsg(buf);
			statue++;
		}
		else if(statue%3 == 1)
		{
			//发湿度
			HMsg(buf,humidity);
			Bluetooth_SendMsg(buf);
			statue++;
		}
		else if(statue%3 == 2)
		{
			//发光强
			LxMsg(buf,lx);
			Bluetooth_SendMsg(buf);
			statue++;
		}
		if(statue == 255)
			statue = 0;
	}
}




void USART3_IRQHandler(void)
{
	uint8_t ch = 0;
	u8 j;
	static int i = 0;
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
#if 0
		usart3_buf[i] = USART_ReceiveData(USART3);
		i++;
		if(i == 9)
		{
			if((usart3_buf[i-9] == 0xff)&&(usart3_buf[i-8] == 0xff))
			{
				usart1_print("rcv ffff\r\n");
				red = usart3_buf[i-4];
				green = usart3_buf[i-3];
				blue = usart3_buf[i-2];
			}
			i = 0;
		}
#endif		
	}
}




