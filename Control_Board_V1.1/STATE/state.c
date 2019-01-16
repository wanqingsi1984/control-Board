#include "main.h"
#include "motor.h"
#include "coor.h"
#include "state.h"
/*******************************************************************
********************************************************************
***********************标定状态相关函数*****************************
********************************************************************
*******************************************************************/


State_TypeDef Current_State = 
{
	CONNECT, 0x00, 0, NOT_CONNECTED, NOT_CONNECTED, UNKNOW_POSITION, UNKNOW_POSITION, AXLE_XP, NO_ERROR
};

State_TypeDef Display_State = 
{
	CONNECT, 0x00, 0, NOT_CONNECTED, NOT_CONNECTED, UNKNOW_POSITION, UNKNOW_POSITION, AXLE_XP, NO_ERROR
};

u8 Display_Flag = 1;

//////////////////////////////////////////
//
//状态功能函数：
//根据当前状态执行不同操作
//2015.07
//by York.Liang
//////////////////////////////////////////
void DealWith_State()
{
// 	if(CONNECTING == Current_State.Device_1||CON_SUCC == Current_State.Device_1||COORDINATING == Current_State.Device_1||COOR_SUCC == Current_State.Device_1)
// 		GPIO_ResetBits(GPIOA,GPIO_Pin_15);
// 	else
// 		POWER1 = OFF;
// 	if(CONNECTING == Current_State.Device_2||CON_SUCC == Current_State.Device_2||COORDINATING == Current_State.Device_2||COOR_SUCC == Current_State.Device_2)
// 		GPIO_ResetBits(GPIOE,GPIO_Pin_9);
// 	else
// 		POWER2 = OFF;
	switch(Current_State.System_State)
	{
		case CONNECT:
				if(CONNECTING == Current_State.Device_1 && 0==Current_State.Time%10)
				{
					Power_Init();
					GPIO_ResetBits(GPIOE,GPIO_Pin_9);
					CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps
					CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps
					delay_ms(500);
					while(CAN1_Send_Msg(Hand_Shake,8));
				}
				if(CONNECTING == Current_State.Device_2 && 0==Current_State.Time%10)
				{
					Power_Init();
					GPIO_ResetBits(GPIOA,GPIO_Pin_15);
					CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps
					CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps 
					delay_ms(500);
					while(CAN2_Send_Msg(Hand_Shake,8));
				}
				if(CONNECTING == Current_State.Device_1 && 200<Current_State.Time)
				{
					Current_State.Device_1 = CON_FAIL;
					POWER1 = OFF;
					Current_State.Sub_State++;
					Current_State.Time = 0;
				}
				if(CONNECTING == Current_State.Device_2 && 200<Current_State.Time)
				{
					Current_State.Device_2 = CON_FAIL;
					POWER2 = OFF;
					Current_State.Sub_State++;
					Current_State.Time = 0;
				}
			break;
		case COORDINATE:
			if(COORDINATING!=Current_State.Device_1 && COOR_SUCC!=Current_State.Device_1
				&& COORDINATING!=Current_State.Device_2 && COOR_SUCC!=Current_State.Device_2)
			{
				Current_State.System_State = RESULT;
				Current_State.Time = 0;
				BEEP_ON;
				delay_ms(50);
				BEEP_OFF;
				delay_ms(50);
				BEEP_ON;
				delay_ms(50);
				BEEP_OFF;
				delay_ms(50);
				BEEP_ON;
				delay_ms(50);
				BEEP_OFF;
			}
			if(NO_ERROR==Current_State.Error_Type)
			{
				switch(Current_State.Sub_State)
				{
					case 0:
						if(Current_State.Time>5||Current_State.Time<10)
							Home();
						if(Current_State.Time>=100&&STEADY)
						{
							Coor_Halt(1);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 1:
						if(Current_State.Time>=60&&STEADY)
						{
							Coor_Spin(1);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 2:
						if(Current_State.Time>=10&&STEADY)
						{
							Move_X(0);
							Current_State.Position_X = MOVING0;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						if(Current_State.Time>=15)
							Current_State.Error_Type |= ERROR_COOR_FAIL;
						break;
					case 3:
						if(Current_State.Time>=70&&STEADY)
						{
							Current_State.Position_X=POSITION1;
							Coor_Halt(2);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 4:
						if(Current_State.Time>=60&&STEADY)
						{
						  Coor_Spin(2);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 5:
						if(Current_State.Time>=10&&STEADY)
						{
							Move_X(1);
							Current_State.Position_X = MOVING0;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						if(Current_State.Time>=15)
							Current_State.Error_Type |= ERROR_COOR_FAIL;
						break;
					case 6:
						if(Current_State.Time>=70&&STEADY)
						{
							Current_State.Position_X=POSITION0;

							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 7:
						if(Current_State.Time>=10&&STEADY)
						{
							Move_X(1);
							Current_State.Position_X = MOVING3;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						if(Current_State.Time>=15)
							Current_State.Error_Type |= ERROR_COOR_FAIL;
						break;
					case 8:
						if(Current_State.Time>=70&&STEADY)
						{
							Current_State.Position_X=POSITION3;
							Coor_Halt(3);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 9:
						if(Current_State.Time>=60&&STEADY)
						{
							Coor_Spin(3);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 10:
						if(Current_State.Time>=10&&STEADY)
						{
							Move_Y(0);
							Current_State.Position_Y = MOVING3;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						if(Current_State.Time>=15)
							Current_State.Error_Type |= ERROR_COOR_FAIL;
						break;
					case 11:
						if(Current_State.Time>=70&&STEADY)
						{
							Current_State.Position_Y=POSITION3;
							Coor_Halt(4);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 12:
						if(Current_State.Time>=60&&STEADY)
						{
							Coor_Spin(4);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 13:
						if(Current_State.Time>=10&&STEADY)
						{
							Move_Y(1);
							Current_State.Position_Y = MOVING3;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						if(Current_State.Time>=15)
							Current_State.Error_Type |= ERROR_COOR_FAIL;
						break;
					case 14:
						if(Current_State.Time>=70&&STEADY)
						{
							Current_State.Position_Y=POSITION0;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 15:
						if(Current_State.Time>=10&&STEADY)
						{
							Move_X(1);
							Current_State.Position_X = MOVING2;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						if(Current_State.Time>=15)
							Current_State.Error_Type |= ERROR_COOR_FAIL;
						break;
					case 16:
						if(Current_State.Time>=70&&STEADY)
						{
							Current_State.Position_X=POSITION2;
							Coor_Halt(5);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 17:
						if(Current_State.Time>=60&&STEADY)
						{
							Coor_Spin(5);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 18:
						if(Current_State.Time>=10&&STEADY)
						{
							Move_Y(1);
							Current_State.Position_Y = MOVING0;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						if(Current_State.Time>=15)
							Current_State.Error_Type |= ERROR_COOR_FAIL;
						break;
					case 19:
						if(Current_State.Time>=70&&STEADY)
						{
							Current_State.Position_Y=POSITION1;
							Coor_Halt(6);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 20:
						if(Current_State.Time>=60&&STEADY)
						{
							Coor_Spin(6);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 21:
						if(Current_State.Time>=10&&STEADY)
						{
							Move_Y(0);
							Current_State.Position_Y = MOVING0;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						if(Current_State.Time>=15)
							Current_State.Error_Type |= ERROR_COOR_FAIL;
						break;
					case 22:
						if(STEADY)
							Current_State.Position_Y=POSITION0;
						if(Current_State.Time==70)
							Send_Coor(Cal_Halt);
						if(Current_State.Time>=100)
						{
							Send_Coor(Cal_Spin);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;		
					case 23:
						if(Current_State.Time>=10&&STEADY)
						{
							Send_Coor(Check_Halt);//静止校验
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 24:
						if(Current_State.Time>=80&&STEADY)
						{
							Move_X(0);
							Current_State.Position_X = MOVING2;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;						
					case 25:
						if(Current_State.Time>=50&&STEADY)
						{
							Current_State.Position_X = POSITION3;
							Send_Coor(Check_Spin_P);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;	
						
					case 26:
						if(Current_State.Time>=10&&STEADY)
						{
							Move_Y(0);
							Current_State.Position_Y = MOVING3;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
						
					case 27:
						if(Current_State.Time>=70&&STEADY)
						{
							Current_State.Position_Y = POSITION3;
							Send_Coor(Check_Spin_Cal);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 28:
						if(Current_State.Time>=10&&STEADY)
						{
							Send_Coor(Check_Spin_N);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 29:
						if(Current_State.Time>=10&&STEADY)
						{
							Move_Y(1);
							Current_State.Position_Y = MOVING3;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;	

					case 30:
						if(Current_State.Time>=70&&STEADY)
						{
							Current_State.Position_Y=POSITION0;
							Send_Coor(Check_Spin_Cal);
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
						
					case 31:
						if(Current_State.Time>=10&&STEADY)
						{
							Current_State.Position_X=MOVING2;
 							Current_State.Position_Y=MOVING3;
							Home();
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;

					case 32:
						if(Current_State.Time>=30&&0==Current_State.Time%10)
						{
								CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps
								CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps
							delay_ms(50);
							if(COORDINATING == Current_State.Device_1)
								while(CAN1_Send_Msg(Result,8));
							else if(COOR_SUCC == Current_State.Device_1)
								while(CAN1_Send_Msg(Save,8));
							else
							{
								Current_State.Sub_State++;
								Current_State.Time=0;
							}
						}
						if(Current_State.Time>=130)
						{							
							Current_State.Device_1 = COOR_FAIL;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					case 33:
						if(Current_State.Time>=10&&0==Current_State.Time%10)
						{
							CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps
							CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps
							delay_ms(50);
							if(COORDINATING == Current_State.Device_2)
								while(CAN2_Send_Msg(Result,8));
							else if(COOR_SUCC == Current_State.Device_2)
								while(CAN2_Send_Msg(Save,8));
							else
							{
								Current_State.Sub_State=0;
								Current_State.Time=0;
							}
						}
						if(Current_State.Time>=110)
						{
							Current_State.Device_2 = COOR_FAIL;
							Current_State.Sub_State++;
							Current_State.Time=0;
						}
						break;
					default:
						break;
				}
			}
			break;
		case RESULT:
			POWER1 = OFF;
			POWER2 = OFF;
			
			break;
		default:
			break;
	}
}

//////////////////////////////////////////
//
//状态显示函数：
//根据当前状态显示LCD
//2015.07
//by York.Liang
//////////////////////////////////////////
void Display_Function()
{
	if(Current_State.Error_Type != Display_State.Error_Type)
	{
		if(Current_State.Error_Type == NO_ERROR)
			Display_Flag = 1;
		else
		{
			BEEP_ON;
			delay_ms(100);
		}
		if((Current_State.Error_Type&ERROR_TIMEOUT) == ERROR_TIMEOUT)
		{
			Show_BMP(ERROR_X,ERROR_Y, TIMEOUT_BMP);
		}
		if((Current_State.Error_Type&ERROR_COOR_FAIL) == ERROR_COOR_FAIL)
		{
			Show_BMP(ERROR_X,ERROR_Y, COORFAIL_BMP);
		}
		if((Current_State.Error_Type&ERROR_NO_CONNECTED) == ERROR_NO_CONNECTED)
		{
			Show_BMP(ERROR_X,ERROR_Y, NOCONNECT_BMP);
		}
		if((Current_State.Error_Type&SINGLE_DEVICE) == SINGLE_DEVICE)
		{
			Show_BMP(ERROR_X,ERROR_Y, SINGLE_BMP);
		}
		if((Current_State.Error_Type&ERROR_KEY) == ERROR_KEY)
		{
			Show_BMP(ERROR_X,ERROR_Y, KEY_BMP);
		}
 		BEEP_OFF;
	}
	
	if((Current_State.System_State != Display_State.System_State) || (1==Display_Flag))
	{
		Display_Flag = 1;
		switch(Current_State.System_State)
		{
			case CONNECT:
				Show_BMP(0,0, CONNECT_BMP);
				break;
			case COORDINATE:
				Show_BMP(0,0, COOR_BMP);
				break;
			case RESULT:
				Show_BMP(0,0, RESULT_BMP);
				break;
			default:
				break;
		}
		delay_ms(10);
	}
	
	if(Current_State.Device_1 != Display_State.Device_1 || 1==Display_Flag)
	{
		switch(Current_State.Device_1)
		{
			case NOT_CONNECTED:
				Show_Rectangle_Color(DEVICE_1_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, GRAY);
				Show_HZ_Color(DEVICE1_TEXT_X, DEVICE_TEXT_Y, 0x04, "未连接  ", BLACK, WHITE);
				break;
			case CONNECTING:
				Show_Rectangle_Color(DEVICE_1_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, GBLUE);
				Show_HZ_Color(DEVICE1_TEXT_X, DEVICE_TEXT_Y, 0x04, "连接中  ", BLACK, WHITE);
				break;
			case CON_SUCC:
				Show_Rectangle_Color(DEVICE_1_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, BLUE);
				Show_HZ_Color(DEVICE1_TEXT_X, DEVICE_TEXT_Y, 0x04, "连接成功", BLACK, WHITE);
				break;
			case CON_FAIL:
				Show_Rectangle_Color(DEVICE_1_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, YELLOW);
				Show_HZ_Color(DEVICE1_TEXT_X, DEVICE_TEXT_Y, 0x04, "连接失败", BLACK, WHITE);
				break;
			case COORDINATING:
				Show_Rectangle_Color(DEVICE_1_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, MAGENTA);
				Show_HZ_Color(DEVICE1_TEXT_X, DEVICE_TEXT_Y, 0x04, "标定中  ", BLACK, WHITE);
				break;
			case COOR_SUCC:
				Show_Rectangle_Color(DEVICE_1_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, BRRED);
				Show_HZ_Color(DEVICE1_TEXT_X, DEVICE_TEXT_Y, 0x04, "存储中  ", BLACK, WHITE);
				break;
			case COOR_FAIL:
				Show_Rectangle_Color(DEVICE_1_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, RED);
				Show_HZ_Color(DEVICE1_TEXT_X, DEVICE_TEXT_Y, 0x04, "标定失败", BLACK, WHITE);
				break;
			case SAVE_SUCC:
				Show_Rectangle_Color(DEVICE_1_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, GREEN);
				Show_HZ_Color(DEVICE1_TEXT_X, DEVICE_TEXT_Y, 0x04, "标定成功", BLACK, WHITE);
				break;
			default:
				break;
		}
	}
		
	if(Current_State.Device_2 != Display_State.Device_2 || 1==Display_Flag)
	{
		switch(Current_State.Device_2)
		{
			case NOT_CONNECTED:
				Show_Rectangle_Color(DEVICE_2_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, GRAY);
				Show_HZ_Color(DEVICE2_TEXT_X, DEVICE_TEXT_Y, 0x04, "未连接  ", BLACK, WHITE);
				break;
			case CONNECTING:
				Show_Rectangle_Color(DEVICE_2_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, GBLUE);
				Show_HZ_Color(DEVICE2_TEXT_X, DEVICE_TEXT_Y, 0x04, "连接中  ", BLACK, WHITE);
				break;
			case CON_SUCC:
				Show_Rectangle_Color(DEVICE_2_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, BLUE);
				Show_HZ_Color(DEVICE2_TEXT_X, DEVICE_TEXT_Y, 0x04, "连接成功", BLACK, WHITE);
				break;
			case CON_FAIL:
				Show_Rectangle_Color(DEVICE_2_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, YELLOW);
				Show_HZ_Color(DEVICE2_TEXT_X, DEVICE_TEXT_Y, 0x04, "连接失败", BLACK, WHITE);
				break;
			case COORDINATING:
				Show_Rectangle_Color(DEVICE_2_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, MAGENTA);
				Show_HZ_Color(DEVICE2_TEXT_X, DEVICE_TEXT_Y, 0x04, "标定中  ", BLACK, WHITE);
				break;
			case COOR_SUCC:
				Show_Rectangle_Color(DEVICE_2_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, BRRED);
				Show_HZ_Color(DEVICE2_TEXT_X, DEVICE_TEXT_Y, 0x04, "存储中  ", BLACK, WHITE);
				break;
			case COOR_FAIL:
				Show_Rectangle_Color(DEVICE_2_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, RED);
				Show_HZ_Color(DEVICE2_TEXT_X, DEVICE_TEXT_Y, 0x04, "标定失败", BLACK, WHITE);
				break;
			case SAVE_SUCC:
				Show_Rectangle_Color(DEVICE_2_X,DEVICE_Y, DEVICE_LEN, DEVICE_LEN, GREEN);
				Show_HZ_Color(DEVICE2_TEXT_X, DEVICE_TEXT_Y, 0x04, "标定成功", BLACK, WHITE);
			default:
				break;
		}
	}
	
	if(Current_State.Position_X != Display_State.Position_X || 1==Display_Flag)
	{
		switch(Current_State.Position_X)
		{
			case POSITION0:
				Show_BMP(MOTOR_X, MOTOR1_Y, ANGLE0);
				break;
			case POSITION1:
				Show_BMP(MOTOR_X, MOTOR1_Y, ANGLE1);
				break;
			case POSITION2:
				Show_BMP(MOTOR_X, MOTOR1_Y, ANGLE2);
				break;
			case POSITION3:
				Show_BMP(MOTOR_X, MOTOR1_Y, ANGLE3);
				break;
			case UNKNOW_POSITION:
				Show_BMP(MOTOR_X, MOTOR1_Y, UNKNOW_ANGLE);
				break;
			case MOVING0:
				Show_BMP(MOTOR_X, MOTOR1_Y, MOVING0_BMP);
				break;
			case MOVING1:
				Show_BMP(MOTOR_X, MOTOR1_Y, MOVING1_BMP);
				break;
			case MOVING2:
				Show_BMP(MOTOR_X, MOTOR1_Y, MOVING2_BMP);
				break;
			case MOVING3:
				Show_BMP(MOTOR_X, MOTOR1_Y, MOVING3_BMP);
				break;
			default:
				break;
		}
	}
	
	if(Current_State.Position_Y != Display_State.Position_Y || 1==Display_Flag)
	{
		switch(Current_State.Position_Y)
		{
			case POSITION0:
				Show_BMP(MOTOR_X, MOTOR2_Y, ANGLE0);
				break;
			case POSITION1:
				Show_BMP(MOTOR_X, MOTOR2_Y, ANGLE1);
				break;
			case POSITION2:
				Show_BMP(MOTOR_X, MOTOR2_Y, ANGLE2);
				break;
			case POSITION3:
				Show_BMP(MOTOR_X, MOTOR2_Y, ANGLE3);
				break;
			case UNKNOW_POSITION:
				Show_BMP(MOTOR_X, MOTOR2_Y, UNKNOW_ANGLE);
				break;
			case MOVING0:
				Show_BMP(MOTOR_X, MOTOR2_Y, MOVING0_BMP);
				break;
			case MOVING1:
				Show_BMP(MOTOR_X, MOTOR2_Y, MOVING1_BMP);
				break;
			case MOVING2:
				Show_BMP(MOTOR_X, MOTOR2_Y, MOVING2_BMP);
				break;
			case MOVING3:
				Show_BMP(MOTOR_X, MOTOR2_Y, MOVING3_BMP);
				break;
			default:
				break;
		}
	}
	
	if(Current_State.Axle != Display_State.Axle || 1==Display_Flag)
	{
		switch(Current_State.Axle)
		{
			case AXLE_XP:
 				Show_BMP(AXLE_X, AXLE_Y, XP_BMP);
				break;
			case AXLE_XN:
				Show_BMP(AXLE_X, AXLE_Y, XN_BMP);
				break;
			case AXLE_YP:
				Show_BMP(AXLE_X, AXLE_Y, YP_BMP);
				break;
			case AXLE_YN:
				Show_BMP(AXLE_X, AXLE_Y, YN_BMP);
				break;
			default:
				break;
		}
	}
	Display_Flag = 0;
	if(Current_State.Sub_State != Display_State.Sub_State&&COORDINATE == Current_State.System_State)
	{
		Show_BMP(34+Current_State.Sub_State*8, BAR_Y, BAR_VALUE);
		if(Current_State.Sub_State==33)
		{
			Show_BMP(30+34*8, BAR_Y, BAR_VALUE);
		}
	}
	Display_State = Current_State;
}