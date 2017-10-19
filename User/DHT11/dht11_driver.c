#include "dht11_driver.h"
#include "SysTick.h"

#define GPIO_PORT_DHT11	GPIOA
#define GPIO_PIN_DHT11	GPIO_Pin_8

#define DHT11_DATA_LOW	GPIO_ResetBits(GPIO_PORT_DHT11,GPIO_PIN_DHT11)
#define DHT11_DATA_HIGH	GPIO_SetBits(GPIO_PORT_DHT11,GPIO_PIN_DHT11)

void DHT11_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_PIN_DHT11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	
	GPIO_SetBits(GPIO_PORT_DHT11, GPIO_PIN_DHT11);
	
	GPIO_Init(GPIO_PORT_DHT11, &GPIO_InitStruct);
}

void DHT11_IO_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_PIN_DHT11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
	GPIO_Init(GPIO_PORT_DHT11, &GPIO_InitStruct);
}

void DHT11_IO_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_PIN_DHT11;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_Init(GPIO_PORT_DHT11, &GPIO_InitStruct);
}

/**开始信号
	返回 0-- 失败
		 1-- 成功
*/
u8 DHT11_Start(void)
{
	u8 wait = 0;
	//主机把总线拉低等待DHT11响应
	DHT11_IO_OUT();
	DHT11_DATA_LOW;
	delay_ms(20);
	//DHT11接受到主机信号后，发送80us低电平响应信号
	DHT11_IO_IN();
	delay_us(30);	//主机发送开始信号结束，必须要延时等待20~40us才能读响应信号
	while((GPIO_ReadInputDataBit(GPIO_PORT_DHT11, GPIO_PIN_DHT11)==1) && wait<90)
	{
		wait++;
		delay_us(1);
	}

	if(wait > 90)
		return 0;
	else wait = 0;
	//再把总线拉高80us，准备发送数据
	while((GPIO_ReadInputDataBit(GPIO_PORT_DHT11, GPIO_PIN_DHT11)==0) && wait<90)
	{
		wait++;
		delay_us(1);
	}

	if(wait > 90)
		return 0;
	else 
		return 1;
}
/*
	读一位
*/
u8 ReadBit(void)
{
	u8 wait = 0;

	while((GPIO_ReadInputDataBit(GPIO_PORT_DHT11, GPIO_PIN_DHT11)==1) && wait<50)
	{
		wait++;
		delay_us(1);
	}
	wait = 0;
	while((GPIO_ReadInputDataBit(GPIO_PORT_DHT11, GPIO_PIN_DHT11)==0) && wait<60)
	{
		wait++;
		delay_us(1);
	}
	delay_us(40);
	
	if(GPIO_ReadInputDataBit(GPIO_PORT_DHT11, GPIO_PIN_DHT11) == 1)
		return 1;
	else 
		return 0;
}
/*
	读一个字节
*/
u8 ReadByte(void)
{
	u8 i = 0;
	u8 byte = 0;
	
	for(; i<8; i++)
	{
		byte <<= 1;
		byte |= ReadBit();
	}
	return byte;
}

/*
	读一次数据
*/
u8 ReadDat(u8 *temp, u8 *tem, u8 *humidity, u8 *hum)
{
	u8 i = 0;
	u8 data[5];
	if(DHT11_Start() == 1)
	{
		for(; i<5; i++)	//读5个字节
		{
			data[i] = ReadByte();
		}
		if((data[0]+data[1]+data[2]+data[3]) == data[4])
		{
			*temp = data[2];
			*tem = data[3];
			*humidity = data[0];
			*hum = data[1];
		}
	}
	else
		return 0;
	return 1;
	
}


