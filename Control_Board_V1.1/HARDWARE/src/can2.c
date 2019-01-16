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
    //Ê¹ÄÜÏà¹ØÊ±ÖÓ
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//Ê¹ÄÜPORTBÊ±ÖÓ	  

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);//Ê¹ÄÜCAN2Ê±ÖÓ	
	
	  //Òı½Å¸´ÓÃÓ³ÉäÅäÖÃ
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2); //GPIOA11¸´ÓÃÎªCAN2
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2); //GPIOA12¸´ÓÃÎªCAN2
	
    //³õÊ¼»¯GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­	
	
    GPIO_Init(GPIOB, &GPIO_InitStructure);//³õÊ¼»¯PA11,PA12
	

	  CAN_DeInit ( CAN2 );
  	//CANµ¥ÔªÉèÖÃ
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//·ÇÊ±¼ä´¥·¢Í¨ĞÅÄ£Ê½   
  	CAN_InitStructure.CAN_ABOM=DISABLE;//DISABLE;	//Èí¼ş×Ô¶¯ÀëÏß¹ÜÀíí	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//Ë¯ÃßÄ£Ê½Í¨¹ıÈí¼ş»½ĞÑ(Çå³ıCAN->MCRµÄSLEEPÎ»)
  	CAN_InitStructure.CAN_NART=DISABLE;//ENABLE;	//½ûÖ¹±¨ÎÄ×Ô¶¯´«ËÍ  
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//±¨ÎÄ²»Ëø¶¨,ĞÂµÄ¸²¸Ç¾ÉµÄ  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//ÓÅÏÈ¼¶ÓÉ±¨ÎÄ±êÊ¶·û¾ö¶¨ 
  	CAN_InitStructure.CAN_Mode= mode;	 //Ä£Ê½ÉèÖÃ 
  	CAN_InitStructure.CAN_SJW=tsjw;	//ÖØĞÂÍ¬²½ÌøÔ¾¿í¶È(Tsjw)Îªtsjw+1¸öÊ±¼äµ¥Î» CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1·¶Î§CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2·¶Î§CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //·ÖÆµÏµÊı(Fdiv)Îªbrp+1	
  	CAN_Init(CAN2, &CAN_InitStructure);   // ³õÊ¼»¯CAN2
    
		//ÅäÖÃ¹ıÂËÆ÷
 	  CAN_FilterInitStructure.CAN_FilterNumber=14;	  //¹ıÂËÆ÷14
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32Î» 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32Î»ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32Î»MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO1;//¹ıÂËÆ÷0¹ØÁªµ½FIFO1
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //¼¤»î¹ıÂËÆ÷14
  	CAN_FilterInit(&CAN_FilterInitStructure);//ÂË²¨Æ÷³õÊ¼»¯
		
#if CAN2_RX0_INT_ENABLE
	
		//NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	  CAN_ITConfig(CAN2,CAN_IT_FMP1,ENABLE);//FIFO0ÏûÏ¢¹ÒºÅÖĞ¶ÏÔÊĞí.		    
 	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ÉèÖÃÏµÍ³ÖĞ¶ÏÓÅÏÈ¼¶·Ö×é2
		
  	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     // Ö÷ÓÅÏÈ¼¶Îª1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // ´ÎÓÅÏÈ¼¶Îª1
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
 
#if CAN2_RX0_INT_ENABLE	//Ê¹ÄÜRX0ÖĞ¶Ï
//ÖĞ¶Ï·şÎñº¯Êı			    
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

//can·¢ËÍÒ»×éÊı¾İ(¹Ì¶¨¸ñÊ½:IDÎª0X12,±ê×¼Ö¡,Êı¾İÖ¡)	
//len:Êı¾İ³¤¶È(×î´óÎª8)				     
//msg:Êı¾İÖ¸Õë,×î´óÎª8¸ö×Ö½Ú.
//·µ»ØÖµ:0,³É¹¦;
//		 ÆäËû,Ê§°Ü;
u8 CAN2_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x1cf394e8;	 // ±ê×¼±êÊ¶·ûÎª0
  TxMessage.ExtId=0x1cf394e8;	 // ÉèÖÃÀ©Õ¹±êÊ¾·û£¨29Î»£©
  TxMessage.IDE=CAN_Id_Extended;		  // Ê¹ÓÃÀ©Õ¹±êÊ¶·û
  TxMessage.RTR=0;		  // ÏûÏ¢ÀàĞÍÎªÊı¾İÖ¡£¬Ò»Ö¡8Î»
  TxMessage.DLC=len;							 // ·¢ËÍÁ½Ö¡ĞÅÏ¢
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // µÚÒ»Ö¡ĞÅÏ¢          
  mbox= CAN_Transmit(CAN2, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN2, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//µÈ´ı·¢ËÍ½áÊø
  if(i>=0XFFF)return 1;
  return 0;

}
//can¿Ú½ÓÊÕÊı¾İ²éÑ¯
//buf:Êı¾İ»º´æÇø;	 
//·µ»ØÖµ:0,ÎŞÊı¾İ±»ÊÕµ½;
//		 ÆäËû,½ÓÊÕµÄÊı¾İ³¤¶È;
u8 CAN2_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN2,CAN_FIFO1)==0)return 0;		//Ã»ÓĞ½ÓÊÕµ½Êı¾İ,Ö±½ÓÍË³ö 
	CAN_Receive(CAN2, CAN_FIFO1, &RxMessage);//¶ÁÈ¡Êı¾İ	
	if( 0x1cf394ea != RxMessage.ExtId) return 0;
	for(i=0;i<RxMessage.DLC;i++)
	buf[i]=RxMessage.Data[i];
	return RxMessage.DLC;	
}














