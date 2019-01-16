#include "sys.h" 


//电机反馈信号
#define READY_X 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0) //PE4
#define ALARM_X 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)	//PE3 
#define HOME_X 		GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4) //PE2
#define TPOS_X 	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)	//PA0

#define READY_Y 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) //PE4
#define ALARM_Y 		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)	//PE3 
#define HOME_Y 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0) //PE2
#define TPOS_Y 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)	//PA0

//电机输出信号
#define ARST_X PAout(8)	
#define SON_X PCout(6)	
#define CTRG_X PCout(7)	
#define POS0_X PCout(8)
#define POS1_X PCout(9)	
#define EMGS_X PCout(11)

#define ARST_Y PBout(6)	
#define SON_Y PDout(7)	
#define CTRG_Y PBout(3)	
#define POS0_Y PBout(4)
#define POS1_Y PBout(5)	
#define EMGS_Y PBout(7)

// #define HUOER PEout(15)

#define STEADY READY_X&&READY_Y
// #define STEADY 1

extern int X_Flag,Y_Flag;  
void Motor_Init(void);
void Stop(void);
void Home(void);
void Move_X(u8 Direction); //0反向,1正向
void Move_Y(u8 Direction); //0反向,1正向

