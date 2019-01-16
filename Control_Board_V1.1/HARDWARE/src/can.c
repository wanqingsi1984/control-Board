#include "can.h"
#include "led.h"
#include "delay.h"
#include "GUI.h"
#include "beep.h"
#include "icon.h"
#include "state.h"
#include "devices.h"

void CAN_Open(int uBaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
// 	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	NVIC_PriorityGroupConfig ( NVIC_PriorityGroup_2 );
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;  //USB �����ȼ����� CAN���� 0 �ж� 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //ʹ��
	NVIC_Init ( &NVIC_InitStructure );  //������ʼ������ NVIC �Ĵ���
	
	
	//NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	NVIC_PriorityGroupConfig ( NVIC_PriorityGroup_2 );
	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;  //USB �����ȼ����� CAN���� 0 �ж� 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;     //ʹ��
	NVIC_Init ( &NVIC_InitStructure );  //������ʼ������ NVIC �Ĵ���
	
	CAN_DeInit ( CAN1 );
	CAN_DeInit ( CAN2 );
	
	CAN_StructInit ( &CAN_InitStructure );

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;       		//����ʹ�ܻ���ʧ��ʱ�䴥��ͨѶģʽ
	CAN_InitStructure.CAN_ABOM = ENABLE;//DISABLE;		//����ʹ�ܻ���ʧ���Զ����߹���
	CAN_InitStructure.CAN_AWUM = DISABLE;       		//����ʹ�ܻ���ʧ���Զ�����ģʽ
	CAN_InitStructure.CAN_NART = DISABLE;       		//����ʹ�ܻ���ʧ�ܷ��Զ��ش���ģʽ
	CAN_InitStructure.CAN_RFLM = DISABLE;       		//����ʹ�ܻ���ʧ�ܽ��� FIFO����ģʽ
	CAN_InitStructure.CAN_TXFP = DISABLE;       		//����ʹ�ܻ���ʧ�ܷ��� FIFO���ȼ�
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;       //����ģʽ
	
	switch(uBaudRate)
	{
		case 5000:
			CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;    //
			CAN_InitStructure.CAN_BS1 = CAN_BS1_10tq;   //
			CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;    //
			CAN_InitStructure.CAN_Prescaler = 2 * 2;    //�����ʼ���36MHz/CAN_Prescaler/(CAN_SJW+CAN_BS1+CAN_BS2) 
			break;

		case 50000:
			CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;    //
			CAN_InitStructure.CAN_BS1 = CAN_BS1_10tq;   //
			CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;    //
			CAN_InitStructure.CAN_Prescaler = 2 * 20;    //�����ʼ���36MHz/CAN_Prescaler/(CAN_SJW+CAN_BS1+CAN_BS2) 
			break;

		case 125000:
			CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;    //
			CAN_InitStructure.CAN_BS1 = CAN_BS1_10tq;   //
			CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;    //
			CAN_InitStructure.CAN_Prescaler = 2 * 8;    //�����ʼ���36MHz/CAN_Prescaler/(CAN_SJW+CAN_BS1+CAN_BS2) 
			break;

		case 250000:
			CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;    //
			CAN_InitStructure.CAN_BS1 = CAN_BS1_10tq;   //
			CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;    //
			CAN_InitStructure.CAN_Prescaler = 2 * 4;    //�����ʼ���36MHz/CAN_Prescaler/(CAN_SJW+CAN_BS1+CAN_BS2) 
			break;

		case 500000:
			CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;    //
			CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;   //
			CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;    //36/(1+10+7)
			CAN_InitStructure.CAN_Prescaler = 3 * 4;    //�����ʼ���36MHz/CAN_Prescaler/(CAN_SJW+CAN_BS1+CAN_BS2) 
			break;

		case 1000000:
			CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;    //zuida 10
			CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;   //
			CAN_InitStructure.CAN_BS2 = CAN_BS2_4tq;    //
			CAN_InitStructure.CAN_Prescaler = 6;    //�����ʼ���36MHz/CAN_Prescaler/(CAN_SJW+CAN_BS1+CAN_BS2) 
			break;

		default:
			CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;    //
			CAN_InitStructure.CAN_BS1 = CAN_BS1_10tq;   //
			CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;    //
			CAN_InitStructure.CAN_Prescaler = 2 * 2;    //�����ʼ���36MHz/CAN_Prescaler/(CAN_SJW+CAN_BS1+CAN_BS2) 
			break;
	}
	CAN_Init ( CAN1, &CAN_InitStructure );
	CAN_Init ( CAN2, &CAN_InitStructure );

	//CAN_Config_Filter(CAN1, ECU_ID_LIST1, 17);
	//CAN_Config_Filter(CAN2, ECU_ID_LIST2, 17);

	CAN_ITConfig ( CAN1, CAN_IT_FMP0, ENABLE );
	CAN_ITConfig ( CAN2, CAN_IT_FMP0, ENABLE );
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOB, &GPIO_InitStructure );
	
	GPIO_ResetBits ( GPIOB, GPIO_Pin_14);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOD, &GPIO_InitStructure );
	
	GPIO_ResetBits ( GPIOD, GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
	
	GPIO_ResetBits ( GPIOA, GPIO_Pin_2);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOA, &GPIO_InitStructure );
	
	GPIO_ResetBits ( GPIOA, GPIO_Pin_3);
}
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	  CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN1_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
    //ʹ�����ʱ��
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��PORTAʱ��	
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
	
			  //���Ÿ���ӳ������
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
	
    //��ʼ��GPIO
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA11,PA12
	

	  
  	//CAN��Ԫ����
   	CAN_InitStructure.CAN_TTCM=DISABLE;	//��ʱ�䴥��ͨ��ģʽ   
  	CAN_InitStructure.CAN_ABOM=DISABLE;//DISABLE;	//����Զ����߹���	  
  	CAN_InitStructure.CAN_AWUM=DISABLE;//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  	CAN_InitStructure.CAN_NART=DISABLE;//ENABLE;	//��ֹ�����Զ����� 
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//���Ĳ�����,�µĸ��Ǿɵ�  
  	CAN_InitStructure.CAN_TXFP=DISABLE;	//���ȼ��ɱ��ı�ʶ������ 
  	CAN_InitStructure.CAN_Mode= mode;	 //ģʽ���� 
  	CAN_InitStructure.CAN_SJW=tsjw;	//����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;  //��Ƶϵ��(Fdiv)Ϊbrp+1	
  	CAN_Init(CAN1, &CAN_InitStructure);   // ��ʼ��CAN1 
    
		//���ù�����
 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //������0
  	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32λ 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32λID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32λMASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
   	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//������0������FIFO0
  	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //���������0
  	CAN_FilterInit(&CAN_FilterInitStructure);//�˲�����ʼ��
		
#if CAN1_RX0_INT_ENABLE
	
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init ( GPIOD, &GPIO_InitStructure );
	
	GPIO_ResetBits ( GPIOD, GPIO_Pin_12);
	
	return 0;
}   
 
#if CAN1_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
void CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	int i=0;
  CAN_Receive(CAN1, 0, &RxMessage);
// 	if(CONNECTING==Current_State.Device_2 && RxMessage.Data[0]==0x1B && RxMessage.Data[1] == 0x02)//(1)//
// 	{
// 		Current_State.Device_2 = CON_SUCC;
// 		Current_State.Sub_State = 0;
// 		Current_State.Time = 0;
// 	}
// 	if(COORDINATING==Current_State.Device_2 && RxMessage.Data[0]==0xFF && RxMessage.Data[1] == 0x0E && RxMessage.Data[2] == 0x01)//(1)//
// 		Current_State.Device_2 = COOR_SUCC;
// 	if(COOR_SUCC==Current_State.Device_2 && RxMessage.Data[0]==0xFF && RxMessage.Data[1] == 0x0F && RxMessage.Data[2] == 0x01)//(1)//
// 	{
// 		Current_State.Device_2 = SAVE_SUCC;
// 		//GPIO_SetBits(GPIOA,GPIO_Pin_15);
// 	}
	for(i=0;i<8;i++);
}
#endif

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x1cf394e8;	 // ��׼��ʶ��Ϊ0
  TxMessage.ExtId=0x1cf394e8;	 // ������չ��ʾ����29λ��
  TxMessage.IDE=CAN_Id_Extended;		  // ʹ����չ��ʶ��
  TxMessage.RTR=0;		  // ��Ϣ����Ϊ����֡��һ֡8λ
  TxMessage.DLC=len;							 // ������֡��Ϣ
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];				 // ��һ֡��Ϣ          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
  if(i>=0XFFF)return 1;
  return 0;
}
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 CAN1_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
	if( 0x1cf394ea != RxMessage.ExtId) return 0;
	for(i=0;i<RxMessage.DLC;i++)
	buf[i]=RxMessage.Data[i];
	return RxMessage.DLC;	
}














