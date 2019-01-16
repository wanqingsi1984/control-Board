#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY1 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1) //PC1
#define KEY2 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_2)	//PC2
#define KEY3 		GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3) //PC3
#define KEY4 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12) //PA1
#define KEY5 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)	//PA2
#define KEY6 		GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3) //PA3

void KEY_Init(void);	//IO初始化
#endif
