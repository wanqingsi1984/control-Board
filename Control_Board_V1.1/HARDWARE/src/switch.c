#include "switch.h"
#include "state.h"
//������ʼ������
void SWITCH_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOFʱ��
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOF2,3,4,5
	
} 

void SWITCH_Scan(void)
{	 
	if(CONNECT==Current_State.System_State)
	{
		if(Pos1==0&&Pos2==0&&Pos3==0&&Pos4==1)
			Current_State.Axle = AXLE_XP;
		else if(Pos1==0&&Pos2==0&&Pos3==1&&Pos4==0)
			Current_State.Axle = AXLE_XN;
		else if(Pos1==0&&Pos2==1&&Pos3==0&&Pos4==0)
			Current_State.Axle = AXLE_YP;
		else if(Pos1==1&&Pos2==0&&Pos3==0&&Pos4==0)
			Current_State.Axle = AXLE_YN;
		else
			Current_State.Axle = AXLE_XP;
	}
}