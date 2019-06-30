#include "sys.h"
#include "usart.h"
#include "data.h"
#include "led.h"
//#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos 使用	  
#endif

#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


/******************串口1函数********************/
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART1_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

u16 USART1_RX_STA1=0;       //接收状态标记	  
u16 USART1_RX_STA2=0; 
u16 USART1_RX_STA3=0; 
u16 DATA_RX_STA=0; 

//u16 DATA_BIT=0;
//初始化IO 串口1 
//bound:波特率
void uart1_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
		USART_DeInit(USART1);  //复位串口1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

    //Usart1 NVIC 配置

		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

		//USART 初始化设置

		USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
    USART_Cmd(USART1, ENABLE);                    //使能串口 

}

void USART1_IRQHandler(void) //液晶屏串口服务程序
{
	u8 Res;
	
	#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
		OSIntEnter();    
	#endif
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //中断产生
	{
			Res = USART_ReceiveData(USART1);//读取接收到的数据	
			/*******数据校验最后三组数据时0xff，用于校验液晶屏发送的数据******/
			if( (USART1_RX_STA1&0x0800)==0&&(USART1_RX_STA2&0x8000)==0&&(USART1_RX_STA3&0x8800)==0 )//接收未完成
			{
				/***************第三步******************/	
				  if( USART1_RX_STA1&0x0400 )
					{
						if(Res==0xAA)
						{
							USART1_RX_BUF[57]=0xAA;
							USART1_RX_STA1=0x0800;
						}
					}
					//////////////////////////////////////
					else if(USART1_RX_STA2&0x3000)		
					{
						if(Res==0xFF)
							USART1_RX_STA2=0x8000;
						else
							USART1_RX_STA2=0;	
					}
					//////////////////////////////////////////
					else if(USART1_RX_STA3&0x0050)		
					{
						if(Res==0xEE)
							USART1_RX_STA3=0x8800;
						else
							USART1_RX_STA3=0;	
					}
					
					
					/***************第二步********************/
					else if( USART1_RX_STA1&0x0200 )
					{
						USART1_RX_STA1++;
						USART1_RX_BUF[USART1_RX_STA1&0X00FF] = Res;
						if((USART1_RX_STA1&0x00FF)==56)
						{
							USART1_RX_STA1|=0x0400;
						}
					}	
					///////////////////////////////////
					else if(USART1_RX_STA2&0x1000)
					{
							if( Res==0xFF )
								USART1_RX_STA2|=0x3000;
							else
								USART1_RX_STA2=0;
					}
					/////////////////////////////////////////
					else if(USART1_RX_STA3&0x0030)
					{
							if( Res==0xEE )
								USART1_RX_STA3|=0x0050;
							else
								USART1_RX_STA3=0;
					}
					
					
					/****************第一步********************/
					else
					{
						if( (Res==0x89)&&(USART1_RX_STA1==0) )
						{
							USART1_RX_STA1|=0x0200;
							USART1_RX_BUF[USART1_RX_STA1&0x00FF] = 0x89;//装载帧头数据
						}
						//////////////////////////////////////
						else if( (Res==0x80)&&(USART1_RX_STA2==0) )//PA
						{
							USART1_RX_STA2|=0x1000;
						}
						//////////////////////////////////////
						else if( (Res==0xFF)&&(USART1_RX_STA3==0) )//IO
						{
							USART1_RX_STA3|=0x0030;
						}
						else
						{
					  	USART1_RX_STA1 = 0;
							USART1_RX_STA2 = 0;
							USART1_RX_STA3 = 0;
						}
					}
			}
   }
	 USART_ClearITPendingBit(USART1, USART_IT_RXNE); //清除中断标志
	 
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif 
}
#endif

/****************串口3函数************************************/
#if EN_USART3_RX   //如果使能了接收

u8 USART3_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

u16 USART3_RX_STA=0;       //接收状态标记	  

void uart3_init(u32 bound)
{
    //GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOB时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //使能USART1
		USART_DeInit(USART3);  //复位串口1
	  //USART1_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化PA9
   
    //USART1_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PA10


   //USART 初始化设置

		USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(USART3, &USART_InitStructure); //初始化串口
#if EN_USART3_RX		  //如果使能了接收  
   //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(USART3, ENABLE);                    //使能串口 

}

void USART3_IRQHandler(void)//上位机服务程序
{
	u8 Res;

#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //中断产生	
	{
		Res = USART_ReceiveData(USART3);
		
		if( (USART3_RX_STA&0x8000)==0 )//接收未完成
		{
			if( USART3_RX_STA&0x4000 )
			{
				if(Res==0x76)						//3、帧尾数据是0x76
				{
					USART3_RX_BUF[24] = 0xaa;
					USART3_RX_STA = 0x8000;  //不需要保留计数功能
				}
				else
					USART3_RX_STA = 0;
			}
			///////////////////第二步////////////////////////////////////
			else if( USART3_RX_STA&0x2000 )
			{
				USART3_RX_STA++;
				USART3_RX_BUF[USART3_RX_STA&0X1FFF] = Res;//2、装载帧数据 
				
				if( (USART3_RX_STA&0x1FFF)==23 )
					USART3_RX_STA|=0x4000;
			}
			/////////////////第一步//////////////////////////////////////
			else
			{
				if( (Res==0x89)&&(USART3_RX_STA==0) )				//1、数据第一个是帧头数据
				{
					Trans_EN = 0;//发送使能端拉低
					USART3_RX_STA |= 0x2000;
					USART3_RX_BUF[USART3_RX_STA&0x1FFF] = 0x89;//装载帧头数据
				}
			}
		}
	}
	
	USART_ClearITPendingBit(USART3, USART_IT_RXNE); //清除中断标志
	
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
	
}
#endif	

/**************************串口4函数*************************/
#if EN_UART4_RX   //如果使能了接收

u8 UART4_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

u16 UART4_RX_STA=0;       //接收状态标记	  

void uart4_init(u32 bound)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//使能UART4，GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
 	USART_DeInit(UART4);  //复位串口4
	 //UART4_TX   PC.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化PA9
   
    //UART4_RX	  PC.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);  //初始化PA10


   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init(UART4, &USART_InitStructure); //初始化串口
#if EN_USART3_RX		  //如果使能了接收  
   //Usart1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(UART4, ENABLE);                    //使能串口 

}

void UART4_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;

#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntEnter();    
#endif
	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) //中断产生	
	{
		Res = USART_ReceiveData(UART4);
		//////////////拉低使能
		if( (UART4_RX_STA&0x8000)==0 )//接收未完成
		{
			if( UART4_RX_STA&0x4000 )
			{
				if(Res==0x76)						//3、帧尾数据是0x76
				{
					UART4_RX_BUF[24] = 0x76;
					UART4_RX_STA|=0x8000;
				}
				else
					UART4_RX_STA = 0;
			}
			///////////////////第二步////////////////////////////////////
			else if( UART4_RX_STA&0x2000 )
			{
				UART4_RX_STA++;
				UART4_RX_BUF[UART4_RX_STA&0X1FFF] = Res;//2、装载帧数据 
				
				if( (UART4_RX_STA&0x1FFF)==23 )
					UART4_RX_STA|=0x4000;
			}
			/////////////////第一步//////////////////////////////////////
			else
			{
				if( (Res==0x89)&&(UART4_RX_STA==0) )				//1、数据第一个是帧头数据
				{
					
					Trans_EN = 0;//发送使能端拉低
					UART4_RX_STA |= 0x2000;
					UART4_RX_BUF[UART4_RX_STA&0x1FFF] = 0x89;//装载帧头数据
				}
			}
		}
	}
	 USART_ClearITPendingBit(UART4, USART_IT_RXNE); //清除中断标志
	
#ifdef OS_TICKS_PER_SEC	 	//如果时钟节拍数定义了,说明要使用ucosII了.
	OSIntExit();  											 
#endif
	
} 
#endif	

