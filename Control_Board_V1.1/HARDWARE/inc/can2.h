#ifndef __CAN2_H
#define __CAN2_H	 
#include "sys.h"	    
#include "GUI.h"
#include "beep.h"
#include "icon.h"
#include "state.h"
#include "devices.h"
	
//CAN1����RX0�ж�ʹ��
#define CAN2_RX0_INT_ENABLE	0		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
 
u8 CAN2_Send_Msg(u8* msg,u8 len);						//��������

u8 CAN2_Receive_Msg(u8 *buf);							//��������
#endif

















