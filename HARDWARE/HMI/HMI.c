#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "HMI.h"
#include "led.h"
#include "flash.h"
#include "data.h"
/*********CPLD的寄存器地址****************/
void HMISends(char *buf1)		  //字符串发送函数
{
	u8 i=0;
	while(1)
	{
		if(buf1[i]!=0)
		{
			USART_SendData(USART1,buf1[i]);  //发送一个字节
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
			i++;
		}
		else break;
	}
	for(i=0;i<3;i++)
	{
		USART_SendData(USART1,0xFF);  //发送一个字节
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
	}
	return ;
}

void HMISendb(u8 k)		         //字节发送函数
{
	u8 i;
	for(i=0;i<3;i++)
	{
		if(k!=0)
		{
			USART_SendData(USART1,k);  //发送一个字节
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//等待发送结束
		}
		else
			return ;
	} 
}

void HMISendstart(void)//为确保串口HMI正常通信
{
	delay_ms(200);
	HMISendb(0xff);
	delay_ms(200);
	HMISendb(0xff);
	delay_ms(200);
}

void HMIRcv(void)//接收函数
{
	static u8 Flag1=0, Flag2=0;   //静态变量
//	char s[10];

	if( USART1_RX_STA3&0x8800 )//功放控制
	{
		Flag2++;
		if(Flag2==1)
		{
			IO = 1;//打开  
			HMISends("pic 150,232,20");
			LED0=1;
		}
		if(Flag2==2)
		{
			IO = 0;//关闭 
			HMISends("pic 150,232,23");
			LED0=0;
			Flag2=0;
		}
		
		USART1_RX_STA3 = 0;//接收标志清零
	}
	
	else if( USART1_RX_STA2&0x8000 )//调制控制
	{
		Flag1++;
		if(Flag1==1)
		{
			Trans_EN = 1;//打开  
			LED0=0;
			HMISends("pic 200,210,21");
		}
		if(Flag1==2)
		{
			Trans_EN = 0;//关闭 
			LED0=1;
			HMISends("pic 200,210,22");

 		  Flag1=0;
		}
		
		USART1_RX_STA2 = 0;//接收标志清零
	}

}

