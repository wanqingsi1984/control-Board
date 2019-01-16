#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "beep.h"
#include "main.h"
#include "motor.h"

int X_Flag = 0,Y_Flag = 0;  
//外部中断0服务程序
void EXTI1_IRQHandler(void)
{
	BEEP_ON;
	delay_ms(50);	//消抖
	BEEP_OFF;
	delay_ms(50);	//消抖
		Home();
    //X_Flag = 1;		 
	
	EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE1上的中断标志位 
}	
//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	BEEP_ON;
	delay_ms(50);	//消抖
	BEEP_OFF;
	delay_ms(50);	//消抖 
	Stop();
	if(COORDINATE == Current_State.System_State)
	{
		if(ERROR_COOR_FAIL == Current_State.Error_Type)
		{
			Current_State.Error_Type &= ~ERROR_COOR_FAIL;
			Current_State.System_State = CONNECT;
			Current_State.Device_1 = NOT_CONNECTED;
			Current_State.Device_2 = NOT_CONNECTED;
			POWER1 = OFF;
			POWER2 = OFF;
		}
		else
		Current_State.Error_Type |= ERROR_COOR_FAIL;
	}
	else 
	{
		if(NO_ERROR == Current_State.Error_Type)
		{
			Current_State.System_State = CONNECT;
			Current_State.Device_1 = NOT_CONNECTED;
			Current_State.Device_2 = NOT_CONNECTED;
			POWER1 = OFF;
			POWER2 = OFF;
		}
	}
	if(ERROR_NO_CONNECTED == Current_State.Error_Type)
		Current_State.Error_Type &= ~ERROR_NO_CONNECTED;
	if(SINGLE_DEVICE == Current_State.Error_Type)
		Current_State.Error_Type &= ~SINGLE_DEVICE;
	 EXTI_ClearITPendingBit(EXTI_Line2);//清除LINE2上的中断标志位 
}
//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	BEEP_ON;
	delay_ms(50);	//消抖
	BEEP_OFF;
	delay_ms(50);	//消抖
	if(EXTI_GetITStatus(EXTI_Line3)==1)	 
	{
		Power_Init();
// 		POWER1=ON;
// 		delay_ms(1000);
		Connect_Device(DEVICE_1);
		EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
	}
}
u8 Outside_Find_Zero_Flag = 0;
void EXTI15_10_IRQHandler(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
	delay_ms(50);	//消抖
	GPIO_ResetBits(GPIOE,GPIO_Pin_11);
	delay_ms(50);	//消抖
	if(EXTI_GetITStatus(EXTI_Line12)==1)	 
	{
		 Power_Init();
		 Connect_Device(DEVICE_2);	
		 EXTI_ClearITPendingBit(EXTI_Line12); 
	}	
	else if(EXTI_GetITStatus(EXTI_Line13)==1)
	{

			EXTI_ClearITPendingBit(EXTI_Line13); 
		
	}
	else if(EXTI_GetITStatus(EXTI_Line14)==1)
	{
				if((CON_SUCC==Current_State.Device_1&&CON_SUCC==Current_State.Device_2)&&NO_ERROR == Current_State.Error_Type)		
		{
			Current_State.System_State = COORDINATE;
				Current_State.Time = 0;
			Current_State.Device_1 = COORDINATING;
			Current_State.Device_2 = COORDINATING;
		}
		else if(CONNECT==Current_State.System_State && 
			(CON_SUCC==Current_State.Device_1||CON_SUCC==Current_State.Device_2)&&
			(NO_ERROR == Current_State.Error_Type||SINGLE_DEVICE == Current_State.Error_Type))
		{
			if(NO_ERROR == Current_State.Error_Type)
				Current_State.Error_Type |= SINGLE_DEVICE;
			else if(SINGLE_DEVICE == Current_State.Error_Type)
			{
				Current_State.Error_Type &= ~SINGLE_DEVICE;
				Current_State.System_State = COORDINATE;
				Current_State.Time = 0;
				if(CON_SUCC==Current_State.Device_1)
					Current_State.Device_1 = COORDINATING;
				if(CON_SUCC==Current_State.Device_2)
					Current_State.Device_2 = COORDINATING;
			}
		}
		else if(CONNECT==Current_State.System_State && CON_SUCC!=Current_State.Device_1&&CON_SUCC!=Current_State.Device_2)
		{
			Current_State.Error_Type |= ERROR_NO_CONNECTED;
		}	
		Y_Flag = 1; 
		
		EXTI_ClearITPendingBit(EXTI_Line14); 
	}
	else if(EXTI_GetITStatus(EXTI_Line15)==1)
	{
			if(1!=Outside_Find_Zero_Flag)
			{
				EMGS_Y = 1;					//停止外框电机
				delay_ms(5);
				EMGS_Y = 0;
			}
			Outside_Find_Zero_Flag = 1;
			
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line15;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
			EXTI_InitStructure.EXTI_LineCmd = DISABLE;//中断线使能
			EXTI_Init(&EXTI_InitStructure);//配置
			
			
			EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
//外部中断初始化程序
//初始化PE2~4,PA0为中断输入.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	KEY_Init(); //按键对应的IO口初始化
 //delay_ms(1000);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);//PC1 连接到中断线1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource2);//PC2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);//PC2 连接到中断线3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource12);//PA1 连接到中断线1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource13);//PA2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource14);//PA3 连接到中断线3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource15);//PA3 连接到中断线3
	
	/* 配置EXTI_Line1,2,3 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line2 | EXTI_Line3| EXTI_Line12|EXTI_Line13|EXTI_Line14|EXTI_Line15;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
  EXTI_Init(&EXTI_InitStructure);//配置
 
	NVIC_PriorityGroupConfig ( NVIC_PriorityGroup_2 );
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//外部中断1
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
// 	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置	   
}












