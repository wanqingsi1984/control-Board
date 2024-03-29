#include "can2.h"
#include "led.h"
#include "delay.h"
#include "GUI.h"
#include "beep.h"
#include "icon.h"
#include "state.h"
#include "devices.h"


u8 CAN2_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN2_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
    //使能相关时钟
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PORTB时钟	  

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//使能CAN2时钟	
	
	  //引脚复用映射配置
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2); //GPIOA11复用为CAN2
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2); //GPIOA12复用为CAN2
	
    //初始化GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉	
	
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化PA11,PA12
	

	  CAN_DeInit ( CAN2 );
  	//CAN单元设置
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//非时间触发通信模式   
  	CAN_InitStructure.CAN_ABOM=DISABLE;//DISABLE;	//软件自动离线管理�	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
  	CAN_InitStructure.CAN_NART=DISABLE;//ENABLE;	//禁止报文自动传送  
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//报文不锁定,新的覆盖旧的  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//优先级由报文标识符决定 
  	CAN_InitStructure.CAN_Mode= mode;	 //模式设置 
  	CAN_InitStructure.CAN_SJW=tsjw;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //分频系数(Fdiv)为brp+1	
  	CAN_Init(CAN2, &CAN_InitStructure);   // 初始化CAN2
    
		//配置过滤器
 	  CAN_FilterInitStructure.CAN_FilterNumber=14;	  //过滤器14
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32位 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32位ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;//过滤器0关联到FIFO1
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活过滤器14
  	CAN_FilterInit(&CAN_FilterInitStructure);//滤波器初始化
		
#if CAN2_RX0_INT_ENABLE
	
		//NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	  CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);//FIFO0消息挂号中断允许.		    
 	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
		
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_Init ( GPIOB, &GPIO_InitStructure );

		GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	return 0;
}   
 
#if CAN2_RX0_INT_ENABLE	//使能RX0中断
//中断服务函数			    
void CAN2_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	int i=0;

	CAN_Receive(CAN2, 0, &RxMessage);
// 	if(CONNECTING==Current_State.Device_1 && RxMessage.Data[0]==0x1B && RxMessage.Data[1] == 0x02)
// 	{
// 		Current_State.Device_1 = CON_SUCC;
// 		Current_State.Sub_State = 0;
// 		Current_State.Time = 0;
// 	}
// 	if(COORDINATING==Current_State.Device_1 && RxMessage.Data[0]==0xFF && RxMessage.Data[1] == 0x0E && RxMessage.Data[2] == 0x01)
// 	{
// 		Current_State.Device_1 = COOR_SUCC;
// 	}
// 	if(COOR_SUCC==Current_State.Device_1 && RxMessage.Data[0]==0xFF && RxMessage.Data[1] == 0x0F && RxMessage.Data[2] == 0x01)
// 	{
// 		Current_State.Device_1 = SAVE_SUCC;
// 	//	GPIO_SetBits(GPIOE,GPIO_Pin_9); 		  
// 	}
	for(i=0;i<8;i++);
}
#endif

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN2_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x1cf394e8;	 // 标准标识符为0
  TxMessage.ExtId=0x1cf394e8;	 // 设置扩展标示符（29位）
  TxMessage.IDE=CAN_Id_Extended;		  // 使用扩展标识符
  TxMessage.RTR=0;		  // 消息类型为数据帧，一帧8位
  TxMessage.DLC=len;							 // 发送两帧信息
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // 第一帧信息          
  mbox= CAN_Transmit(CAN2, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
  if(i>=0XFFF)return 1;
  return 0;

}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN2_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN2,CAN_FIFO1)==0)return 0;		//没有接收到数据,直接退出 
	CAN_Receive(CAN2, CAN_FIFO1, &RxMessage);//读取数据	
	if( 0x1cf394ea != RxMessage.ExtId) return 0;
	for(i=0;i<RxMessage.DLC;i++)
	buf[i]=RxMessage.Data[i];
	return RxMessage.DLC;	
}














