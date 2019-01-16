#include "sys.h" 

#define Pos1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PF2
#define Pos2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PF3 
#define Pos3 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PF4
#define Pos4 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5)	//PF5

void SWITCH_Init(void);	//IO初始化
void SWITCH_Scan(void);  		//按键扫描函数	
