#ifndef __LCD_H
#define __LCD_H

#include "stm32f4xx.h"
#include "icon.h"
//GUI指令
#define SET_BACK				0x81
#define SET_FRONT				0x82

//画笔颜色
#define BACK_COLOR			 0XF79E	
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	   0x001F  
#define BRED             0XF81F
#define GRED 			 			 0XFFE0
#define GBLUE			 			 0X07FF
#define RED           	 0xF800
#define ORANGE					 0xF4C0
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 		 0XBC40 //棕色
#define BRRED 					 0XFC07 //棕红色
#define GRAY  					 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 					 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

extern u16 Front_Color,Back_Color;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// 单一指令//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Uart_Send_LCD_CMD(USART_TypeDef* USARTx, u8* data);
void Set_COLOR(u16 COLOR, u8 CMD);
void Clear_LCD();
void Show_ASCII_Str(u16 Loc_X,u16 Loc_Y, u8 FT, u8* str);
void Show_HZ_Str(u16 Loc_X,u16 Loc_Y, u8 FT, u8* str);
void Show_BMP(u16 Loc_X,u16 Loc_Y, u16 icon_add);
void Draw_Rectangle(u16 Loc_XS,u16 Loc_YS, u16 Width, u16 Height);
void Draw_Box(u16 Loc_XS,u16 Loc_YS, u16 Width, u16 Height);
void Draw_Button(u16 Loc_XS,u16 Loc_YS, u16 Width, u16 Height, u8 mode);
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// 组合指令//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Show_ASCII_Color(u16 Loc_X,u16 Loc_Y, u8 FT, u8* str, u16 Front_COLOR, u16 Back_COLOR);
void Show_HZ_Color(u16 Loc_X,u16 Loc_Y, u8 FT, u8* str, u16 Front_COLOR, u16 Back_COLOR);
void Show_Rectangle_Color(u16 Loc_XS,u16 Loc_YS, u16 Width, u16 Height, u16 COLOR);
#endif
