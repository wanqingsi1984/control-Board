#include "exti.h"
#include "delay.h" 
#include "led.h" 
#include "key.h"
#include "beep.h"
#include "main.h"
#include "motor.h"

int X_Flag = 0,Y_Flag = 0;  
//�ⲿ�ж�0�������
void EXTI1_IRQHandler(void)
{
	BEEP_ON;
	delay_ms(50);	//����
	BEEP_OFF;
	delay_ms(50);	//����
		Home();
    //X_Flag = 1;		 
	
	EXTI_ClearITPendingBit(EXTI_Line1); //���LINE1�ϵ��жϱ�־λ 
}	
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	BEEP_ON;
	delay_ms(50);	//����
	BEEP_OFF;
	delay_ms(50);	//���� 
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
	 EXTI_ClearITPendingBit(EXTI_Line2);//���LINE2�ϵ��жϱ�־λ 
}
//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	BEEP_ON;
	delay_ms(50);	//����
	BEEP_OFF;
	delay_ms(50);	//����
	if(EXTI_GetITStatus(EXTI_Line3)==1)	 
	{
		Power_Init();
// 		POWER1=ON;
// 		delay_ms(1000);
		Connect_Device(DEVICE_1);
		EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
	}
}
u8 Outside_Find_Zero_Flag = 0;
void EXTI15_10_IRQHandler(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_SetBits(GPIOE,GPIO_Pin_11);
	delay_ms(50);	//����
	GPIO_ResetBits(GPIOE,GPIO_Pin_11);
	delay_ms(50);	//����
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
				EMGS_Y = 1;					//ֹͣ�����
				delay_ms(5);
				EMGS_Y = 0;
			}
			Outside_Find_Zero_Flag = 1;
			
			
			EXTI_InitStructure.EXTI_Line = EXTI_Line15;
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
			EXTI_InitStructure.EXTI_LineCmd = DISABLE;//�ж���ʹ��
			EXTI_Init(&EXTI_InitStructure);//����
			
			
			EXTI_ClearITPendingBit(EXTI_Line15);
	}
}
//�ⲿ�жϳ�ʼ������
//��ʼ��PE2~4,PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	KEY_Init(); //������Ӧ��IO�ڳ�ʼ��
 //delay_ms(1000);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);//PC1 ���ӵ��ж���1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource2);//PC2 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);//PC2 ���ӵ��ж���3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource12);//PA1 ���ӵ��ж���1
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource13);//PA2 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource14);//PA3 ���ӵ��ж���3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource15);//PA3 ���ӵ��ж���3
	
	/* ����EXTI_Line1,2,3 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line2 | EXTI_Line3| EXTI_Line12|EXTI_Line13|EXTI_Line14|EXTI_Line15;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
 
	NVIC_PriorityGroupConfig ( NVIC_PriorityGroup_2 );
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�ⲿ�ж�1
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
// 	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//�ⲿ�ж�3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����	   
}












