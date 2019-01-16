#include "motor.h"
#include "state.h"
extern u8 Outside_Find_Zero_Flag;
void Motor_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
	///////////////////////////////////////电机驱动反馈信号线/////////////////////////////////////////////////////
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOF时钟
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOD0,3,4,5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB8,9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
  GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE0,1
	
	////////////////////////////////////////电机驱动控制信号线//////////////////////////////////////////////////////
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOF时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	GPIO_ResetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
	GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);
	GPIO_ResetBits(GPIOC,GPIO_Pin_11);
	
	
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	GPIO_ResetBits(GPIOD,GPIO_Pin_7);
	
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
//   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
//   GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOD0,3,4,5
}

void Stop(void)
{
	//while(1)//(!(STEADY))
	{
		if(1)//(!(READY_X))
		{
			EMGS_X = 1;
			delay_ms(5);
			EMGS_X = 0;
			Current_State.Position_X=UNKNOW_POSITION;
		}
		if(1)//(!(READY_Y))
		{
			EMGS_Y = 1;
			delay_ms(5);
			EMGS_Y = 0;
			Current_State.Position_Y=UNKNOW_POSITION;
		}
	}
}
u8 First_Time_Zeros_Flag = 0;
int Zeros_Count = 0;
void Home(void)
{
	if(0 == First_Time_Zeros_Flag)
	{
		First_Time_Zeros_Flag = 1;

		if(X_Flag == 0)
		{
			POS0_X = 0;
			POS1_X = 0;
			delay_ms(5);
			CTRG_X = 1;
			delay_ms(5);
			CTRG_X = 0;
			while(!(STEADY));

			
			for(;;)
			{
				if(1==Outside_Find_Zero_Flag||Zeros_Count>10)
					break;
				POS0_Y = 0;
				POS1_Y = 1;
				delay_ms(5);
				CTRG_Y = 1;
				delay_ms(5);
				CTRG_Y = 0;
				delay_ms(500);
				Zeros_Count++;
				if(Zeros_Count>10000)
					Zeros_Count = 0;
				
			}
			if(Y_Flag == 0)
			{
				POS0_Y = 0;
				POS1_Y = 0;
				delay_ms(5);
				CTRG_Y = 1;
				delay_ms(5);
				CTRG_Y = 0;
				while(!(STEADY));
			}
		}
	}
	else
	{
		POS0_X = 1;
		POS1_X = 1;
		POS0_Y = 1;
		POS1_Y = 1;
		delay_ms(5);
		CTRG_X = 1;
		CTRG_Y = 1;
		delay_ms(5);
		CTRG_X = 0;
		CTRG_Y = 0;
		while(!(STEADY));
	}
	Current_State.Position_X=POSITION0;
	Current_State.Position_Y=POSITION0;
}

void Move_X(u8 Direction) //0反向,1正向
{
	if(1==Direction)
	{
			GPIO_SetBits(GPIOC,GPIO_Pin_8);
			GPIO_ResetBits(GPIOC,GPIO_Pin_9);
	}
	else
	{
			GPIO_ResetBits(GPIOC,GPIO_Pin_8);
			GPIO_SetBits(GPIOC,GPIO_Pin_9);
	}
	delay_ms(5);
	GPIO_SetBits(GPIOC,GPIO_Pin_7);
	delay_ms(5);
	GPIO_ResetBits(GPIOC,GPIO_Pin_7);
//	POS0_X = Direction;
//	POS1_X =!Direction;
//	delay_ms(5);
//	CTRG_X = 1;
//	delay_ms(5);
//	CTRG_X = 0;
}

void Move_Y(u8 Direction) //0反向,1正向
{
		if(1==Direction)
		{
				GPIO_SetBits(GPIOB,GPIO_Pin_4);
				GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		}
		else
		{
				GPIO_ResetBits(GPIOB,GPIO_Pin_4);
				GPIO_SetBits(GPIOB,GPIO_Pin_5);
		}
		delay_ms(5);
		GPIO_SetBits(GPIOB,GPIO_Pin_3);
		delay_ms(5);
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
//	POS0_Y = Direction;
//	POS1_Y = !Direction;
//	delay_ms(5);
//	CTRG_Y = 1;
//	delay_ms(5);
//	CTRG_Y = 0;
}