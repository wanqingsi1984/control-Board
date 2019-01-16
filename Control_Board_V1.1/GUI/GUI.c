#include	"usart.h"
#include	"usart5.h"
#include	"sys.h"
#include	"GUI.h"
#include "delay.h"

u16 Front_Color,Back_Color;
///////////////////////////////////
//����β�����ۼ�У��ֵ��������
//���룺USARTx ���ںţ�data ���ݣ�length ����
//2015.07
//by York.Liang
///////////////////////////////////
void Uart_Send_LCD_CMD(USART_TypeDef* USARTx, u8* data)
{
	u8 t;
	u8 CheckSum = 0;
	
// 	while(!(1==UART5_RX_BUF[2]&&data[2]==UART5_RX_BUF[3]))
// 	{
		for(t=0;t<data[1];t++)
		{
			USART_SendData(USARTx, data[t]);//�򴮿�1��������
			while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			CheckSum += data[t];
		}
		USART_SendData(USARTx, CheckSum);//�򴮿�1��������
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
// 		delay_ms(10);
// 	}
	Clear_RX5_BUF();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// ��һָ��//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////
//����ǰ/����ɫ
//���룺COLOR ��ɫ��CMD ǰ/����ɫ
//2015.07
//by York.Liang
///////////////////////////////////
void Set_COLOR(u16 COLOR, u8 CMD)
{
	u8 data[5] = {0xaa, 0x05, SET_BACK, 0x00, 0x00};
	if((SET_FRONT == CMD && Front_Color == COLOR)||(SET_BACK == CMD && Back_Color == COLOR))
  		return;
	if(SET_FRONT == CMD)
		data[2] = CMD;
	data[3] = COLOR >> 8;
	data[4] = COLOR & 0xFF;
	Uart_Send_LCD_CMD(UART5, data);
	if(SET_BACK == CMD)
		Back_Color = COLOR;
	else if(SET_FRONT == CMD)
		Front_Color = COLOR;
}

///////////////////////////////////
//����
//2015.07
//by York.Liang
///////////////////////////////////
void Clear_LCD(void)
{
	u8 data[4] = {0xaa, 0x04, 0x80, 0x00};
	Uart_Send_LCD_CMD(UART5, data);
}

///////////////////////////////////
//ASCII�ַ�����ʾ
//���룺 
//Loc_X/Loc_Y ����
//FT ��4λ 0xf ͸��д�ַ� 0x0 ������ɫд�ַ�  
//   ��4λ �����д1:8*16 2:12*24 3:16*32
//str �ַ���ָ��
//length ��ʾ�ַ����ĳ���1~20
//2015.07
//by York.Liang
///////////////////////////////////
void Show_ASCII_Str(u16 Loc_X,u16 Loc_Y, u8 FT, u8* str)
{
	u8 flag=1;
	u8 data[29] = {0x00};
	data[0] = 0xaa;
	data[1] = 0x08;
	data[2] = 0x86;
	data[3] = Loc_X >> 8;
	data[4] = Loc_X & 0xFF;
	data[5] = Loc_Y >> 8;
	data[6] = Loc_Y & 0xFF;
	data[7] = FT;
	while(flag)
	{
		data[data[1]++] = *str++;
		if(29==data[1]||*str == '\0')
			flag=0;
	}
	Uart_Send_LCD_CMD(UART5, data);
}

///////////////////////////////////
//�����ַ�����ʾ
//���룺 
//Loc_X/Loc_Y ����
//FT ��4λ 0xf ͸��д�ַ� 0x0 ������ɫд�ַ�  
//   ��4λ �����д4:16*16 5:24*24 6:32*32
//str ������λ���ַ���ָ��
//length ��ʾ�ַ����ĳ���1~20
//2015.07
//by York.Liang
///////////////////////////////////
void Show_HZ_Str(u16 Loc_X,u16 Loc_Y, u8 FT, u8* str)
{
	u8 flag=1;
	u8 data[29] = {0x00};
	data[0] = 0xaa;
	data[1] = 0x08;
	data[2] = 0x88;
	data[3] = Loc_X >> 8;
	data[4] = Loc_X & 0xFF;
	data[5] = Loc_Y >> 8;
	data[6] = Loc_Y & 0xFF;
	data[7] = FT;
	while(flag)
	{
		
		data[data[1]++] = *str++;
		if(29==data[1]||*str == '\0')
			flag=0;
	}
	Uart_Send_LCD_CMD(UART5, data);
}

///////////////////////////////////
//BMPͼƬ��ʾ
//���룺 
//Loc_X/Loc_Y ����
//icon_addͼƬ��ַ��������icon.h
//2015.07
//by York.Liang
///////////////////////////////////
void Show_BMP(u16 Loc_X,u16 Loc_Y, u16 icon_add)
{
	u8 data[11] = {0x00};
	data[0] = 0xaa;
	data[1] = 0x0b;
	data[2] = 0x89;
	data[3] = Loc_X >> 8;
	data[4] = Loc_X & 0xFF;
	data[5] = Loc_Y >> 8;
	data[6] = Loc_Y & 0xFF;
	data[9] = icon_add >> 8;
	data[10] = icon_add & 0xFF; 
	Uart_Send_LCD_CMD(UART5, data);
}

///////////////////////////////////
//��ʵ�ľ���
//���룺 
//Loc_XS/Loc_YS ��ʼ����
//Width ���/Height �߶�
//2015.07
//by York.Liang
///////////////////////////////////
void Draw_Rectangle(u16 Loc_XS,u16 Loc_YS, u16 Width, u16 Height)
{
	u8 data[12] = {0x00};
	data[0] = 0xaa;
	data[1] = 0x0b;
	data[2] = 0x8d;
	data[3] = Loc_XS >> 8;
	data[4] = Loc_XS & 0xFF;
	data[5] = Loc_YS >> 8;
	data[6] = Loc_YS & 0xFF;
	data[7] = Width >> 8;
	data[8] = Width & 0xFF;
	data[9] = Height >> 8;
	data[10] = Height & 0xFF;
	Uart_Send_LCD_CMD(UART5, data);
}

///////////////////////////////////
//������
//���룺 
//Loc_XS/Loc_YS ��ʼ����
//Width ���/Height �߶�
//2015.07
//by York.Liang
///////////////////////////////////
void Draw_Box(u16 Loc_XS,u16 Loc_YS, u16 Width, u16 Height)
{
	u8 data[12] = {0x00};
	data[0] = 0xaa;
	data[1] = 0x0c;
	data[2] = 0x9a;
	data[3] = Loc_XS >> 8;
	data[4] = Loc_XS & 0xFF;
	data[5] = Loc_YS >> 8;
	data[6] = Loc_YS & 0xFF;
	data[7] = Width >> 8;
	data[8] = Width & 0xFF;
	data[9] = Height >> 8;
	data[10] = Height & 0xFF;
	Uart_Send_LCD_CMD(UART5, data);
}

///////////////////////////////////
//���Ի���
//���룺 
//Loc_XS/Loc_YS ��ʼ����
//Width ���/Height �߶�
//2015.07
//by York.Liang
///////////////////////////////////
void Draw_Button(u16 Loc_XS,u16 Loc_YS, u16 Width, u16 Height, u8 mode)
{
	u8 data[12] = {0x00};
	data[0] = 0xaa;
	data[1] = 0x0c;
	data[2] = 0x9b;
	data[3] = Loc_XS >> 8;
	data[4] = Loc_XS & 0xFF;
	data[5] = Loc_YS >> 8;
	data[6] = Loc_YS & 0xFF;
	data[7] = (Loc_XS+Width) >> 8;
	data[8] = (Loc_XS+Width) & 0xFF;
	data[9] = (Loc_YS+Height) >> 8;
	data[10] = (Loc_YS+Height) & 0xFF;
	data[11] = mode;
	Uart_Send_LCD_CMD(UART5, data);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// ���ָ��//////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Show_ASCII_Color(u16 Loc_X,u16 Loc_Y, u8 FT, u8* str, u16 Front_COLOR, u16 Back_COLOR)
{
	Set_COLOR(Front_COLOR, SET_FRONT);
	if(FT>>4 == 0x0)
		Set_COLOR(Back_COLOR, SET_BACK);
	Show_ASCII_Str(Loc_X,Loc_Y, FT, str);
}

void Show_HZ_Color(u16 Loc_X,u16 Loc_Y, u8 FT, u8* str, u16 Front_COLOR, u16 Back_COLOR)
{
	Set_COLOR(Front_COLOR, SET_FRONT);
	if(FT>>4 == 0x0)
		Set_COLOR(Back_COLOR, SET_BACK);
	Show_HZ_Str(Loc_X,Loc_Y, FT, str);
}

void Show_Rectangle_Color(u16 Loc_XS,u16 Loc_YS, u16 Width, u16 Height, u16 COLOR)
{
	Set_COLOR(COLOR, SET_FRONT);
	Draw_Rectangle(Loc_XS, Loc_YS, Width, Height);
}
