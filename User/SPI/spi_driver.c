#include "spi_driver.h"
#include "usart_driver.h"


void SPI_InitConfig(void)
{
	#if 0
	/****硬件设置SPI****/
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;	//方向:1线制的Tx(操作OLCD)
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//工作在master主机模式下
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 //每次传输8bit(扩展的SPI支持16bit)
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		 	//CPOL相位: 高
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		//CPHA极性:1 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  	//软件来设置片选线
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	//波特率分频系数 8分频
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  	//首位，第一位先发高位(很少用低位)
	SPI_InitStructure.SPI_CRCPolynomial = 7;		//校验(一般选7)
	SPI_Init(SPI1, &SPI_InitStructure);
	
	// Enable SPI1  
	SPI_Cmd(SPI1, ENABLE);
	#else
	/****软件模拟SPI****/
	GPIO_InitTypeDef GPIO_InitStructure;
//	printf("SPI_InitConfig(void)\r\n");
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);  
	
	#endif
}

#if 0
/****硬件设置SPI写数据***/
void SPI_WriterByte(u8 dat)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	// 通过 SPI2发送一字节数据 
	SPI_I2S_SendData(SPI1, dat);	
}
#endif

#if 1
/****软件设置SPI写数据***/
#define OLED_SCLK_Clr()		GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define OLED_SCLK_Set()		GPIO_SetBits(GPIOC, GPIO_Pin_4)

#define OLED_SDIN_Clr()		GPIO_ResetBits(GPIOC, GPIO_Pin_5)	
#define OLED_SDIN_Set()		GPIO_SetBits(GPIOC, GPIO_Pin_5)


void SPI_WriterByte(u8 dat)
{	
	u8 i;			  
	//这里只写数据
	for(i=0; i<8; i++)
	{
		OLED_SCLK_Clr();
		if(dat & 0x80)
			OLED_SDIN_Set();
		else
			OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat <<= 1;
	}
}
#endif







