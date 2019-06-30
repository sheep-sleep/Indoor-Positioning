#include "ADF435X.h"
#include "spi.h"
#include "flash.h"
#include "delay.h" 
#include "led.h"
//�̶����137.5001MHZ�ļĴ�������
#define R0 0X2C0010
#define R1 0X800E1A9
#define R2 0X18004E42
#define R3 0X4B3
#define R4 0XCC803C
#define R5 0X580005	

//float Atten;//˥��Attendb

unsigned long R4_V;//�洢��������R4��ֵ

static unsigned long Frequence;	//Ƶ�ʣ���λKHZ   200000MHZ
int  Amplitude[2] = {3,3};				//���ȣ�����ͨ��
unsigned int phase = 90;					//��λ���λ ��

void WriteToADF435X(unsigned long BUF)
{
	unsigned char i = 0;
  unsigned long ValueToWrite = 0;
	
  ValueToWrite =BUF;
  CLK=0;
	LE=0;
  for(i=0;i<32;i++)
  {
		if(0x80000000==(ValueToWrite & 0x80000000))
		{
			DATA=1;//Send one to SDO pin
			delay_us(10);
		}
    else
		{
			DATA=0;//Send zero to SDO pin   
		  delay_us(10);		
		}
    CLK=1;
    ValueToWrite <<= 1; //Rotate data
		delay_us(10);
    CLK=0;
  }
  CLK=0;
  LE=1;
	delay_us(10);
	LE=0;
}

void  Write_frequence(unsigned long Freq)	 //137500KHZ
{
	float Freq_temp;
	unsigned long 	INT,FRAC,MODE=3125; //������Ƶ��С����Ƶ  ȡģֵ MODE=3125;	//MODE=3125��ϵͳ����25MHZ���񣬷ֱ���Ϊ1KHZ����С�ֱ��ʿ���Ϊ0.5KHZ
	unsigned long   R0_temp=0;  //����INT  FRAC
	unsigned long   R4_temp=0; //�ı�����ķ�Ƶ�����֣�һ���ֳ�4��   2.2GHZ--4.4GHZ	   1.1GHZ--2.2GHZ      550MHZ--1.1GHZ   275MHZ--550MHZ   137.5MHZ--275MHZ
	unsigned long   Out_Divider,RF_Divider;//�����Ƶ���ƣ�3λ  ����Ӧ�ķ�Ƶ�� 
	
	if((34375<=Freq)  &&(Freq<68750))  {Out_Divider=6;RF_Divider=64;}   //ADF4351���� 
	if((68750<=Freq)  &&(Freq<137500)) {Out_Divider=5;RF_Divider=32;}   //ADF4351����                                                          
	if((137500<=Freq) &&(Freq<275000)) {Out_Divider=4;RF_Divider=16;}  //��ͬƵ�β��ò�ͬ�ķ�Ƶ ADF4351 50���� 
	if((275000<=Freq) &&(Freq<550000)) {Out_Divider=3;RF_Divider=8;}
	if((550000<=Freq) &&(Freq<1100000)){Out_Divider=2;RF_Divider=4;}
	if((1100000<=Freq)&&(Freq<2200000)){Out_Divider=1;RF_Divider=2;}
	if((2200000<=Freq)&&(Freq<4400000)){Out_Divider=0;RF_Divider=1;}
	
	R4_temp = ((R4_V&0XFF8FFFFF)|(Out_Divider<<20));
	WriteToADF435X(R4_temp);
	Freq_temp = Freq;   //ת��Ƶ�ʵõ�С����Ƶ��������Ƶ
	Freq_temp = (Freq_temp*RF_Divider)/100000;  ////////////////
  INT  = (int)Freq_temp;
	FRAC = (Freq_temp-INT)*MODE;
  R0_temp = ((INT<<15)|(FRAC<<3))&0x7fffff8;
	WriteToADF435X(R0_temp);
	R4_V = R4_temp;
}

void Write_Amplitude(unsigned char Amp)
{
	unsigned long Amp_temp,R4_temp;
	Amp_temp = Amp;
	R4_temp = ((R4_V&0XFFFFFFE7)|(Amp_temp<<3));
	WriteToADF435X(R4_temp);
	R4_V = R4_temp;
}

void Init_ADF435X(void)
{
	 WriteToADF435X(R0);
	 WriteToADF435X(R1);
	 WriteToADF435X(R2);
	 WriteToADF435X(R3);
	 WriteToADF435X(R4);
	 WriteToADF435X(R5);   
	 
	 WriteToADF435X(R0);
	 WriteToADF435X(R1);
	 WriteToADF435X(R2);
	 WriteToADF435X(R3);
	 WriteToADF435X(R4);
	 WriteToADF435X(R5);
	 
	 R4_V=R4;
}

void Set_Voltage_Gain(float Gain)
{
	u8 Gain_temp = 0;
	
	Gain_temp = (u8)(Gain*2.0);
	
//	C0_5 = 1;	 //0000_0001
//	C1_0 = 1;  //0000_0010
//	C2_0 = 1;  //0000_0100
//	C4_0 = 1;  //0000_1000
//	C8_0 = 1;  //0001_0000
//	C16_0= 1;	 //0010_0000

	C0_5 = (u8)((Gain_temp&0x01)>>0);	 //0000_0001
	C1_0 = (u8)((Gain_temp&0x02)>>1);  //0000_0010
	C2_0 = (u8)((Gain_temp&0x04)>>2);  //0000_0100
	C4_0 = (u8)((Gain_temp&0x08)>>3);  //0000_1000
	C8_0 = (u8)((Gain_temp&0x10)>>4);  //0001_0000
	C16_0= (u8)((Gain_temp&0x20)>>5);	 //0010_0000
}

