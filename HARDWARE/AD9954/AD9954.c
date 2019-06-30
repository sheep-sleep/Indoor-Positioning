
#include "AD9954.h"
#include "delay.h"
#include "led.h"

void send_data(u8 dat )
{

 	u8 i;
	for(i=0;i<8;i++)
	{
		 AD9954_SCLK=0;	  
		 if(dat&0x80)
		 {
			  AD9954_SDIO=1;
		 }
		 else
		 {
			  AD9954_SDIO=0;
		 }
		 AD9954_SCLK=1;
		 dat<<= 1;
	 }

}

void Init_AD9954(void) //初始化AD9952使输出为1MHZ
{
	AD9954_CS=0;
	IOUPDATE=0;
	AD9954_RES = 1;
  delay_ms(5);
	AD9954_RES = 0;	
	 
	send_data(0x00);//00
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x42);
	
	send_data(0x01);//01
	send_data(0x00);
	send_data(0x00);
	send_data(0xA4);
	
	send_data(0x02);//02
	send_data(0x3f);
	send_data(0xff);

	
	send_data(0x03);//03
	send_data(0x00);
	
	send_data(0x04);//04
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	
	send_data(0x05);//05
	send_data(0x00);
	send_data(0x00);
	
	IOUPDATE = 1;
	delay_ms(5);
	IOUPDATE = 0;
}


void set_freq(float f) // 单独设置固定频率值；

 {
 
 	u32 date; 
	AD9954_CS=0;
	   
	date=(u32)(10.7374*f);//det=(f/fclk)x2^32=10.7374xf

	send_data(0x04);

  send_data((u8)(date>>24));//频率控制字
	send_data((u8)(date>>16));
	send_data((u8)(date>>8));
	send_data((u8)date);

	IOUPDATE = 1;
	delay_ms(10);
  IOUPDATE = 0;

}
void set_2psk(float f1,float phase1,float phase2)
{
  u16 date;
	
  AD9954_CS=0;
  IOUPDATE = 0;
	
	send_data(0x00);//00
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x40);
	
	send_data(0x01);//01
	send_data(0x00);
	send_data(0x00);
	send_data(0xA7);
	
	set_freq(f1);
	
  PS0=0;
  PS1=0;
	send_data(0x07);//07
	send_data(0x01);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
	send_data(0x00);
  IOUPDATE = 1;
	delay_ms(10);
  IOUPDATE = 0;
	
  date=45.51*phase1;
	date=date<<2;
	send_data(0x0b);
	send_data((u8)(date>>8));//频率控制字
	send_data((u8)date);
	send_data(0x00);
	send_data(0x00);
  
	
  PS0=1;//ram1
	PS1=0;
	send_data(0x08);//08
	send_data(0x01);
	send_data(0x00);
	send_data(0x01);
	send_data(0x04);
	send_data(0x00);
	IOUPDATE = 1;
	delay_ms(10);
  IOUPDATE = 0;
	
  date=45.51*phase2;
	date=date<<2;
	send_data(0x0b);
	send_data((u8)(date>>8));//频率控制字
	send_data((u8)date);
	send_data(0x00);
	send_data(0x00);  

//	
//	PS0=0;//ram1
//	PS1=1;
//	send_data(0x09);//09
//	send_data(0x01);
//	send_data(0x00);
//	send_data(0x02);
//	send_data(0x08);
//	send_data(0x00);
//	IOUPDATE = 1;
//	delay_ms(10);
//  IOUPDATE = 0;
//		
//  date=45.51*phase2;
//	date=date<<2;
//	send_data(0x0b);
//	send_data((u8)(date>>8));//频率控制字
//	send_data((u8)date);
//	send_data(0x00);
//	send_data(0x00);


//	PS0=1;//ram1
//	PS1=1;
//	send_data(0x0a);//0a
//	send_data(0x01);
//	send_data(0x00);
//	send_data(0x03);
//	send_data(0x0c);
//	send_data(0x00);
//	IOUPDATE = 1;
//	delay_ms(10);
//  IOUPDATE = 0;
//		
//  date=45.51*phase2;
//	date=date<<2;
//	send_data(0x0b);
//	send_data((u8)(date>>8));//频率控制字
//	send_data((u8)date);
//	send_data(0x00);
//	send_data(0x00);
//	IOUPDATE = 1;
//	delay_ms(10);
//  IOUPDATE = 0;


	send_data(0x00);//地址0写操作
	send_data(0xc0);//打开RAM控制位驱动FTW
	send_data(0x00);
  send_data(0x00);
	send_data(0x40);
}
