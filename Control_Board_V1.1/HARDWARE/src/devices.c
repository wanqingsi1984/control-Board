#include "main.h"

void Connect_Device(u8 Device)
{
	if(CONNECT==Current_State.System_State && NO_ERROR==Current_State.Error_Type && 
			CONNECTING!=Current_State.Device_1 && CONNECTING != Current_State.Device_2)
	{
		if(DEVICE_1 == Device)
		{
			Current_State.Time = 1;
			Current_State.Device_1 = CONNECTING;
		}
		else if(DEVICE_2 == Device)
		{
			Current_State.Time = 1;
			Current_State.Device_2 = CONNECTING;
		}
	}	
}

void Power_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��	
	//GPIO_SetBits(GPIOA,GPIO_Pin_15);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��	
	//GPIO_SetBits(GPIOE,GPIO_Pin_9);
}