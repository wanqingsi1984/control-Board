#include "main.h"
#include "switch.h"
u8 can1buf[8] = {0};
u8 can2buf[8] = {0};
void Process_Data(void)
{

	u8 CAN1_flag,CAN2_flag;	
	
	CAN1_flag = CAN1_Receive_Msg(can1buf);
	CAN2_flag = CAN2_Receive_Msg(can2buf);
	if(CAN1_flag)
	{
		if(CONNECTING==Current_State.Device_1 && can1buf[0]==0x1B && can1buf[1] == 0x02)
		{
			Current_State.Device_1 = CON_SUCC;
			Current_State.Sub_State = 0;
			Current_State.Time = 0;
		}
		if(COORDINATING==Current_State.Device_1 && can1buf[0]==0xFF && can1buf[1] == 0x0E && can1buf[2] == 0x01)
		{
			Current_State.Device_1 = COOR_SUCC;
		}
		if(COOR_SUCC==Current_State.Device_1 && can1buf[0]==0xFF && can1buf[1] == 0x0F && can1buf[2] == 0x01)
		{
			Current_State.Device_1 = SAVE_SUCC;
		//	GPIO_SetBits(GPIOE,GPIO_Pin_9); 		  
		}
	}
	if(CAN2_flag)
	{
		if(CONNECTING==Current_State.Device_2 && can2buf[0]==0x1B && can2buf[1] == 0x02)//(1)//
		{
			Current_State.Device_2 = CON_SUCC;
			Current_State.Sub_State = 0;
			Current_State.Time = 0;
		}
		if(COORDINATING==Current_State.Device_2 && can2buf[0]==0xFF && can2buf[1] == 0x0E && can2buf[2] == 0x01)//(1)//
			Current_State.Device_2 = COOR_SUCC;
		if(COOR_SUCC==Current_State.Device_2 && can2buf[0]==0xFF && can2buf[1] == 0x0F && can2buf[2] == 0x01)//(1)//
		{
			Current_State.Device_2 = SAVE_SUCC;
 			//GPIO_SetBits(GPIOA,GPIO_Pin_15);
		}
		
	}
}

void CAN_Open(int uBaudRate);
extern u8 Cal_Halt[8];
u16 KEY_cnt = 0;
int a1,al_x,home_x,tpod,B1,B2,HUOER;
int main(void)
{ 
//  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
 	LED_BEEP_Init();			     //LED,BEEP端口初始化
	SWITCH_Init();

 	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps  
 	CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_5tq,6,CAN_Mode_Normal);//CAN初始化环回模式,波特率500Kbps  
  //	CAN_Open(1000000);
	uart5_init(9600);	 //串口初始化为9600

	BEEP_ON;
	delay_ms(100);
 	Set_COLOR(BACK_COLOR, SET_BACK);
	Set_COLOR(RED, SET_FRONT);
	
	delay_ms(100);
  Show_BMP(390, 0, CLC_BMP);
	
 	Show_BMP(AXLE_X, AXLE_Y, XP_BMP); 
	EXTIX_Init();		 				//外部中断初始化
	delay_ms(10);
 	TIM3_Int_Init(500-1,12000-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数1000次为100ms 
	//Power_Init();//设备上电初始化
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
	Motor_Init();
// 	Current_State.Device_1 = CON_SUCC;
// 	Current_State.Device_2 = CON_SUCC;
	delay_ms(10);
	GPIO_SetBits(GPIOC,GPIO_Pin_6);
	GPIO_SetBits(GPIOD,GPIO_Pin_7);
	
	BEEP_OFF;
	while(1)
	{
// 		a1 = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0); //PE4
// 		al_x = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3);	//PE3 
// 		home_x = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4); //PE2
// 		tpod = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5);	//PA0
// 		B1 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8); //PE2
//     B2 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9);	//PA0
// 		HUOER = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15);	//PA0
		Process_Data();		
		delay_ms(100);
		if(KEY1==0 || KEY2==0 || KEY3==0 || KEY4==0 || KEY5==0)		//三个按键中有一个按下
			KEY_cnt++;
		else
			KEY_cnt = 0;
		if(KEY_cnt>=100)
		{
			KEY_cnt=100;	//防止计数过大
			Current_State.Error_Type |= ERROR_KEY;
		}
		else
		{
			Current_State.Error_Type &= ~ERROR_KEY;
		}
		SWITCH_Scan();
		LED0 = !LED0;
	}
}

