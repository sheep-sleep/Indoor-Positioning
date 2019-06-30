#include "sys.h"
#include "usart.h"
#include "data.h"
#include "led.h"
//#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif

#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 


/******************����1����********************/
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART1_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

u16 USART1_RX_STA1=0;       //����״̬���	  
u16 USART1_RX_STA2=0; 
u16 USART1_RX_STA3=0; 
u16 DATA_RX_STA=0; 

//u16 DATA_BIT=0;
//��ʼ��IO ����1 
//bound:������
void uart1_init(u32 bound){
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
		USART_DeInit(USART1);  //��λ����1
	 //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

    //Usart1 NVIC ����

		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

		//USART ��ʼ������

		USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART1, &USART_InitStructure); //��ʼ������
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

void USART1_IRQHandler(void) //Һ�������ڷ������
{
	u8 Res;
	
	#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
		OSIntEnter();    
	#endif
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�жϲ���
	{
			Res = USART_ReceiveData(USART1);//��ȡ���յ�������	
			/*******����У�������������ʱ0xff������У��Һ�������͵�����******/
			if( (USART1_RX_STA1&0x0800)==0&&(USART1_RX_STA2&0x8000)==0&&(USART1_RX_STA3&0x8800)==0 )//����δ���
			{
				/***************������******************/	
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
					
					
					/***************�ڶ���********************/
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
					
					
					/****************��һ��********************/
					else
					{
						if( (Res==0x89)&&(USART1_RX_STA1==0) )
						{
							USART1_RX_STA1|=0x0200;
							USART1_RX_BUF[USART1_RX_STA1&0x00FF] = 0x89;//װ��֡ͷ����
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
	 USART_ClearITPendingBit(USART1, USART_IT_RXNE); //����жϱ�־
	 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif 
}
#endif

/****************����3����************************************/
#if EN_USART3_RX   //���ʹ���˽���

u8 USART3_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

u16 USART3_RX_STA=0;       //����״̬���	  

void uart3_init(u32 bound)
{
    //GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//GPIOBʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //ʹ��USART1
		USART_DeInit(USART3);  //��λ����1
	  //USART1_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PA9
   
    //USART1_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PA10


   //USART ��ʼ������

		USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(USART3, &USART_InitStructure); //��ʼ������
#if EN_USART3_RX		  //���ʹ���˽���  
   //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
#endif
    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}

void USART3_IRQHandler(void)//��λ���������
{
	u8 Res;

#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //�жϲ���	
	{
		Res = USART_ReceiveData(USART3);
		
		if( (USART3_RX_STA&0x8000)==0 )//����δ���
		{
			if( USART3_RX_STA&0x4000 )
			{
				if(Res==0x76)						//3��֡β������0x76
				{
					USART3_RX_BUF[24] = 0xaa;
					USART3_RX_STA = 0x8000;  //����Ҫ������������
				}
				else
					USART3_RX_STA = 0;
			}
			///////////////////�ڶ���////////////////////////////////////
			else if( USART3_RX_STA&0x2000 )
			{
				USART3_RX_STA++;
				USART3_RX_BUF[USART3_RX_STA&0X1FFF] = Res;//2��װ��֡���� 
				
				if( (USART3_RX_STA&0x1FFF)==23 )
					USART3_RX_STA|=0x4000;
			}
			/////////////////��һ��//////////////////////////////////////
			else
			{
				if( (Res==0x89)&&(USART3_RX_STA==0) )				//1�����ݵ�һ����֡ͷ����
				{
					Trans_EN = 0;//����ʹ�ܶ�����
					USART3_RX_STA |= 0x2000;
					USART3_RX_BUF[USART3_RX_STA&0x1FFF] = 0x89;//װ��֡ͷ����
				}
			}
		}
	}
	
	USART_ClearITPendingBit(USART3, USART_IT_RXNE); //����жϱ�־
	
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
	
}
#endif	

/**************************����4����*************************/
#if EN_UART4_RX   //���ʹ���˽���

u8 UART4_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.

u16 UART4_RX_STA=0;       //����״̬���	  

void uart4_init(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��UART4��GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
 	USART_DeInit(UART4);  //��λ����4
	 //UART4_TX   PC.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��PA9
   
    //UART4_RX	  PC.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);  //��ʼ��PA10


   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init(UART4, &USART_InitStructure); //��ʼ������
#if EN_USART3_RX		  //���ʹ���˽���  
   //Usart1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//�����ж�
#endif
    USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ��� 

}

void UART4_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;

#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) //�жϲ���	
	{
		Res = USART_ReceiveData(UART4);
		//////////////����ʹ��
		if( (UART4_RX_STA&0x8000)==0 )//����δ���
		{
			if( UART4_RX_STA&0x4000 )
			{
				if(Res==0x76)						//3��֡β������0x76
				{
					UART4_RX_BUF[24] = 0x76;
					UART4_RX_STA|=0x8000;
				}
				else
					UART4_RX_STA = 0;
			}
			///////////////////�ڶ���////////////////////////////////////
			else if( UART4_RX_STA&0x2000 )
			{
				UART4_RX_STA++;
				UART4_RX_BUF[UART4_RX_STA&0X1FFF] = Res;//2��װ��֡���� 
				
				if( (UART4_RX_STA&0x1FFF)==23 )
					UART4_RX_STA|=0x4000;
			}
			/////////////////��һ��//////////////////////////////////////
			else
			{
				if( (Res==0x89)&&(UART4_RX_STA==0) )				//1�����ݵ�һ����֡ͷ����
				{
					
					Trans_EN = 0;//����ʹ�ܶ�����
					UART4_RX_STA |= 0x2000;
					UART4_RX_BUF[UART4_RX_STA&0x1FFF] = 0x89;//װ��֡ͷ����
				}
			}
		}
	}
	 USART_ClearITPendingBit(UART4, USART_IT_RXNE); //����жϱ�־
	
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
	
} 
#endif	

