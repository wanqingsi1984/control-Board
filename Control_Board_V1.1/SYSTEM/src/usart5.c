#include "sys.h"
#include "usart5.h"	

 
#if EN_UART5_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 UART5_RX_BUF[UART5_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 UART5_RX_STA=0;       //����״̬���	

//��ʼ��IO ����1 
//bound:������
void uart5_init(u32 bound){
   //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOC12����ΪUART5
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOD2����ΪUART5
	
	//UART5�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIOC12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC12
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PD2

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(UART5, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(UART5, ENABLE);  //ʹ�ܴ���1 
	
	USART_ClearFlag(UART5, USART_FLAG_TC);
	
#if EN_UART5_RX	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif
	
}

u8 len5;
u8 cnt5=0;
void UART5_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res, Check_Sum;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//��ȡ���յ�������
		
		if(UART5_RX_STA == 0)								//ͨѶ��ͷ
		{
			if(Res == 0xaa)
			{
				UART5_RX_STA++;
				UART5_RX_BUF[cnt5++] = Res;
				Check_Sum=0xaa;
			}
		}
		else if(UART5_RX_STA == 1)					//ͨѶ����
		{
			UART5_RX_BUF[cnt5++] = Res;
			Check_Sum+=Res;
			len5 = Res;
			if(len5<20)												//�޶����ݳ���С��20
				UART5_RX_STA++;
			else
			{
				UART5_RX_STA=0;
				cnt5=0;
			}
		}
		else if(UART5_RX_STA == 2)					//�洢����
		{
			UART5_RX_BUF[cnt5++] = Res;
			Check_Sum+=Res;
			if(cnt5==len5)
			{
				UART5_RX_STA++;
			}
		}
		else if(UART5_RX_STA == 3)					//CRC-L��֤
		{
			if(Res == Check_Sum)
 				UART5_RX_STA++;
			else
			{
				UART5_RX_STA=0;
				cnt5 = 0;
			}
		}   		 
  } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
}

///////////////////////////////////
//��ս������ݻ���
//2015.07
//by York.Liang
///////////////////////////////////
void Clear_RX5_BUF()
{
	int i;
	for(i=0;i<=len5;i++)
		UART5_RX_BUF[i]=0;
}

#endif	

 



