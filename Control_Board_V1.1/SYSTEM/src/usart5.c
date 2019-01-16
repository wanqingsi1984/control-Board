#include "sys.h"
#include "usart5.h"	

 
#if EN_UART5_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 UART5_RX_BUF[UART5_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 UART5_RX_STA=0;       //接收状态标记	

//初始化IO 串口1 
//bound:波特率
void uart5_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOC12复用为UART5
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOD2复用为UART5
	
	//UART5端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIOC12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC12
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOD2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
// 	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PD2

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(UART5, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(UART5, ENABLE);  //使能串口1 
	
	USART_ClearFlag(UART5, USART_FLAG_TC);
	
#if EN_UART5_RX	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

#endif
	
}

u8 len5;
u8 cnt5=0;
void UART5_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Res, Check_Sum;
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//读取接收到的数据
		
		if(UART5_RX_STA == 0)								//通讯字头
		{
			if(Res == 0xaa)
			{
				UART5_RX_STA++;
				UART5_RX_BUF[cnt5++] = Res;
				Check_Sum=0xaa;
			}
		}
		else if(UART5_RX_STA == 1)					//通讯长度
		{
			UART5_RX_BUF[cnt5++] = Res;
			Check_Sum+=Res;
			len5 = Res;
			if(len5<20)												//限定数据长度小于20
				UART5_RX_STA++;
			else
			{
				UART5_RX_STA=0;
				cnt5=0;
			}
		}
		else if(UART5_RX_STA == 2)					//存储数据
		{
			UART5_RX_BUF[cnt5++] = Res;
			Check_Sum+=Res;
			if(cnt5==len5)
			{
				UART5_RX_STA++;
			}
		}
		else if(UART5_RX_STA == 3)					//CRC-L验证
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
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
}

///////////////////////////////////
//清空接收数据缓存
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

 



