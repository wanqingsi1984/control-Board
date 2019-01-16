#include "led.h" 

//初始化PF9和PF10为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_BEEP_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIO时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_11;
	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_6);
	GPIO_SetBits(GPIOE,GPIO_Pin_7);
	GPIO_ResetBits(GPIOE,GPIO_Pin_11);
}

void Beep_ms(u8 ms)
{
// 	BEEP = 1;
	delay_ms(ms);
// 	BEEP = 0;
}




