#ifndef _OLED_DRIVER_H_
#define _OLED_DRIVER_H_

#include "stm32f10x.h"

void LCD_Reset(void);
void LCD_Set_Pos(unsigned char x, unsigned char y);
void LCD_Fill(unsigned char bmp_dat);
void LCD_CLS(void);

void LCD_Init(void);
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);
void Draw_BMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char bmp[]);

u32 oled_pow(u8 m,u8 n);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);

#endif
