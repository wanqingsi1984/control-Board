#ifndef __CAN2_H
#define __CAN2_H	 
#include "sys.h"	    
#include "GUI.h"
#include "beep.h"
#include "icon.h"
#include "state.h"
#include "devices.h"
	
//CAN1接收RX0中断使能
#define CAN2_RX0_INT_ENABLE	0		//0,不使能;1,使能.								    
										 							 				    
u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
 
u8 CAN2_Send_Msg(u8* msg,u8 len);						//发送数据

u8 CAN2_Receive_Msg(u8 *buf);							//接收数据
#endif

















