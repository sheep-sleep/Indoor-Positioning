#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "HMI.h"
#include "led.h"
#include "flash.h"
#include "data.h"
/*********CPLD�ļĴ�����ַ****************/
void HMISends(char *buf1)		  //�ַ������ͺ���
{
	u8 i=0;
	while(1)
	{
		if(buf1[i]!=0)
		{
			USART_SendData(USART1,buf1[i]);  //����һ���ֽ�
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
			i++;
		}
		else break;
	}
	for(i=0;i<3;i++)
	{
		USART_SendData(USART1,0xFF);  //����һ���ֽ�
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
	}
	return ;
}

void HMISendb(u8 k)		         //�ֽڷ��ͺ���
{
	u8 i;
	for(i=0;i<3;i++)
	{
		if(k!=0)
		{
			USART_SendData(USART1,k);  //����һ���ֽ�
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET){};//�ȴ����ͽ���
		}
		else
			return ;
	} 
}

void HMISendstart(void)//Ϊȷ������HMI����ͨ��
{
	delay_ms(200);
	HMISendb(0xff);
	delay_ms(200);
	HMISendb(0xff);
	delay_ms(200);
}

void HMIRcv(void)//���պ���
{
	static u8 Flag1=0, Flag2=0;   //��̬����
//	char s[10];

	if( USART1_RX_STA3&0x8800 )//���ſ���
	{
		Flag2++;
		if(Flag2==1)
		{
			IO = 1;//��  
			HMISends("pic 150,232,20");
			LED0=1;
		}
		if(Flag2==2)
		{
			IO = 0;//�ر� 
			HMISends("pic 150,232,23");
			LED0=0;
			Flag2=0;
		}
		
		USART1_RX_STA3 = 0;//���ձ�־����
	}
	
	else if( USART1_RX_STA2&0x8000 )//���ƿ���
	{
		Flag1++;
		if(Flag1==1)
		{
			Trans_EN = 1;//��  
			LED0=0;
			HMISends("pic 200,210,21");
		}
		if(Flag1==2)
		{
			Trans_EN = 0;//�ر� 
			LED0=1;
			HMISends("pic 200,210,22");

 		  Flag1=0;
		}
		
		USART1_RX_STA2 = 0;//���ձ�־����
	}

}

