#include "coor.h"
#include "state.h"
#include "can.h"
#include "can2.h"
#include "led.h"

/****************************************标定指令*****************************************/

u8 Hand_Shake[8] = {0x1a,0x02,0x55,0x55,0x55,0x55,0x55,0x55};

u8 Spin_XP[8] = {0xee,0x01,0x00,0x00,0x00,0x5a,0x01,SPIN_TIME};
u8 Spin_XN[8] = {0xee,0x02,0x00,0x00,0x00,0x5a,0x01,SPIN_TIME};
u8 Spin_YP[8] = {0xee,0x03,0x00,0x00,0x00,0x5a,0x01,SPIN_TIME};
u8 Spin_YN[8] = {0xee,0x04,0x00,0x00,0x00,0x5a,0x01,SPIN_TIME};
u8 Spin_ZP[8] = {0xee,0x05,0x00,0x00,0x00,0x5a,0x01,SPIN_TIME};
u8 Spin_ZN[8] = {0xee,0x06,0x00,0x00,0x00,0x5a,0x01,SPIN_TIME};
u8 Halt_XP[8] = {0xee,0x07,HALT_TIME,0x55,0x55,0x55,0x55,0x55};
u8 Halt_XN[8] = {0xee,0x08,HALT_TIME,0x55,0x55,0x55,0x55,0x55};
u8 Halt_YP[8] = {0xee,0x09,HALT_TIME,0x55,0x55,0x55,0x55,0x55};
u8 Halt_YN[8] = {0xee,0x0a,HALT_TIME,0x55,0x55,0x55,0x55,0x55};
u8 Halt_ZP[8] = {0xee,0x0b,HALT_TIME,0x55,0x55,0x55,0x55,0x55};
u8 Halt_ZN[8] = {0xee,0x0c,HALT_TIME,0x55,0x55,0x55,0x55,0x55};

u8 Cal_Halt[8] = {0xee,0x14,0x55,0x55,0x55,0x55,0x55,0x55};
u8 Cal_Spin[8] = {0xee,0x12,0x55,0x55,0x55,0x55,0x55,0x55};

u8 Check_Halt[8] = {0xee,0x0d,0x01,0x55,0x55,0x55,0x55,0x55};
u8 Check_Spin_P[8] = {0xee,0x0d,0x02,0x55,0x55,0x55,0x55,0x55};
u8 Check_Spin_N[8] = {0xee,0x0d,0x03,0x55,0x55,0x55,0x55,0x55};
u8 Check_Spin_Cal[8] = {0xee,0x0d,0x04,0x55,0x55,0x55,0x55,0x55};

u8 Result[8] = {0xEE,0x0e,0x55,0x55,0x55,0x55,0x55,0x55};
u8 Save[8] = {0xEE,0x0f,0x55,0x55,0x55,0x55,0x55,0x55};

void Send_Coor(u8* Data)
{
// 	BEEP = 1;
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps
	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps 
	delay_ms(100);
	while(CAN2_Send_Msg(Data,8));
	delay_ms(50);
	while(CAN1_Send_Msg(Data,8));
	delay_ms(50);
// 	BEEP = 0;
}

void Coor_Halt(u8 Num)
{
	u8* Send_Data;
 	switch (Num)
	{
		case 1:
			Send_Data = Halt_ZN;
			break;
		case 2:
			switch(Current_State.Axle)
			{
				case AXLE_XP:
					Send_Data = Halt_XP;
					break;
				case AXLE_XN:
					Send_Data = Halt_XN;
					break;
				case AXLE_YP:
					Send_Data = Halt_YP;
					break;
				case AXLE_YN:
					Send_Data = Halt_YN;
					break;
				default:
					break;
			}
			break;
		case 3:
			switch(Current_State.Axle)
			{
				case AXLE_XP:
					Send_Data = Halt_XN;
					break;
				case AXLE_XN:
					Send_Data = Halt_XP;
					break;
				case AXLE_YP:
					Send_Data = Halt_YN;
					break;
				case AXLE_YN:
					Send_Data = Halt_YP;
					break;
				default:
					break;
			}
			break;
		case 4:
			switch(Current_State.Axle)
			{
				case AXLE_XP:
					Send_Data = Halt_YP;
					break;
				case AXLE_XN:
					Send_Data = Halt_YN;
					break;
				case AXLE_YP:
					Send_Data = Halt_XN;
					break;
				case AXLE_YN:
					Send_Data = Halt_XP;
					break;
				default:
					break;
			}
			break;
		case 5:
			Send_Data = Halt_ZP;
			break;
		case 6:
			switch(Current_State.Axle)
			{
				case AXLE_XP:
					Send_Data = Halt_YN;
					break;
				case AXLE_XN:
					Send_Data = Halt_YP;
					break;
				case AXLE_YP:
					Send_Data = Halt_XP;
					break;
				case AXLE_YN:
					Send_Data = Halt_XN;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	Send_Coor(Send_Data);
}

void Coor_Spin(u8 Num)
{
	u8* Send_Data;
	switch (Num)
	{
		case 1:
			switch(Current_State.Axle)
			{
				case AXLE_XP:
					Send_Data = Spin_YP;
					break;
				case AXLE_XN:
					Send_Data = Spin_YN;
					break;
				case AXLE_YP:
					Send_Data = Spin_XN;
					break;
				case AXLE_YN:
					Send_Data = Spin_XP;
					break;
				default:
					break;
			}
			break;
		case 2:
			switch(Current_State.Axle)
			{
				case AXLE_XP:
					Send_Data = Spin_YN;
					break;
				case AXLE_XN:
					Send_Data = Spin_YP;
					break;
				case AXLE_YP:
					Send_Data = Spin_XP;
					break;
				case AXLE_YN:
					Send_Data = Spin_XN;
					break;
				default:
					break;
			}
			break;
		case 3:
			Send_Data = Spin_ZP;
			break;
		case 4:
			Send_Data = Spin_ZN;
			break;
		case 5:
			switch(Current_State.Axle)
			{
				case AXLE_XP:
					Send_Data = Spin_XN;
					break;
				case AXLE_XN:
					Send_Data = Spin_XP;
					break;
				case AXLE_YP:
					Send_Data = Spin_YN;
					break;
				case AXLE_YN:
					Send_Data = Spin_YP;
					break;
				default:
					break;
			}
			break;
		case 6:
			switch(Current_State.Axle)
			{
				case AXLE_XP:
					Send_Data = Spin_XP;
					break;
				case AXLE_XN:
					Send_Data = Spin_XN;
					break;
				case AXLE_YP:
					Send_Data = Spin_YP;
					break;
				case AXLE_YN:
					Send_Data = Spin_YN;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	Send_Coor(Send_Data);
}

