#include "spi_driver.h"
#include "usart_driver.h"


void SPI_InitConfig(void)
{
	#if 0
	/****Ӳ������SPI****/
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;	//����:1���Ƶ�Tx(����OLCD)
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//������master����ģʽ��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 //ÿ�δ���8bit(��չ��SPI֧��16bit)
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		 	//CPOL��λ: ��
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		//CPHA����:1 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		  	//���������Ƭѡ��
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;	//�����ʷ�Ƶϵ�� 8��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  	//��λ����һλ�ȷ���λ(�����õ�λ)
	SPI_InitStructure.SPI_CRCPolynomial = 7;		//У��(һ��ѡ7)
	SPI_Init(SPI1, &SPI_InitStructure);
	
	// Enable SPI1  
	SPI_Cmd(SPI1, ENABLE);
	#else
	/****���ģ��SPI****/
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
/****Ӳ������SPIд����***/
void SPI_WriterByte(u8 dat)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	// ͨ�� SPI2����һ�ֽ����� 
	SPI_I2S_SendData(SPI1, dat);	
}
#endif

#if 1
/****�������SPIд����***/
#define OLED_SCLK_Clr()		GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define OLED_SCLK_Set()		GPIO_SetBits(GPIOC, GPIO_Pin_4)

#define OLED_SDIN_Clr()		GPIO_ResetBits(GPIOC, GPIO_Pin_5)	
#define OLED_SDIN_Set()		GPIO_SetBits(GPIOC, GPIO_Pin_5)


void SPI_WriterByte(u8 dat)
{	
	u8 i;			  
	//����ֻд����
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







