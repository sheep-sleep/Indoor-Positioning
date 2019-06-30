#include "data.h"
#include "usart.h"
#include "HMI.h"
#include "flash.h" 
//#include "delay.h"
#include "led.h"
#include "ADF435X.h"	
#include "stmflash.h"

u8 REG_ADDR = 0x00;//д�Ĵ����׵�ַ
u8 REG_ADDR_READ = 0x19;//���Ĵ����׵�ַ
u8 ReadStatusAddr =0x1A;//д���ݽ�����־λ�Ĵ�����ַ
u8 Save_Data[25];
u8 Save_Data_LCD[25];

#define SIZE sizeof(Save_Data) //����Ԫ�ظ���
////Ҫд�뵽STM32 FLASH���ַ�������
u8 datatemp[SIZE];//���ݻ�����
unsigned long R4_V_temp = 0xA6403C;

#define FLASH_SAVE_ADDR  0X08070000 //����FLASH �����ַ(����Ϊż��)

void INTEGRATE_DATA(void)
{
	 u8 i=0,j=0;
	 u8 temp=0x89; 
	
	 if( USART3_RX_STA==0x8000 )//�������ݴ���
	 {
		 for(i=0;i<23;i++)					//��������
			 Save_Data[i] = USART3_RX_BUF[i];
		 
		 for (i = 1; i < 23; i++)
		 {
			 temp ^= Save_Data[i];
		 }
		
		 Save_Data[23] = temp;//żУ����
		 Save_Data[24] = USART3_RX_BUF[24];

		 USART3_RX_STA=0xFF00;
	 }
	
	 if( USART1_RX_STA1==0x0800 )//Һ�����ݴ���
	 {		
		Save_Data_LCD[0]  = USART1_RX_BUF[0];//֡ͷ
		Save_Data_LCD[1]  = USART1_RX_BUF[1];//ʶ����
		Save_Data_LCD[2]  = USART1_RX_BUF[6];
		Save_Data_LCD[3]  = USART1_RX_BUF[5];
		Save_Data_LCD[4]  = USART1_RX_BUF[9];
		Save_Data_LCD[5]  = USART1_RX_BUF[14];
		Save_Data_LCD[6]  = USART1_RX_BUF[13];
		Save_Data_LCD[7]  = USART1_RX_BUF[17];
		Save_Data_LCD[8]  = USART1_RX_BUF[22];
		Save_Data_LCD[9]  = USART1_RX_BUF[21];
		Save_Data_LCD[10] = USART1_RX_BUF[25];
		Save_Data_LCD[11] = USART1_RX_BUF[30];
		Save_Data_LCD[12] = USART1_RX_BUF[29];
		Save_Data_LCD[13] = USART1_RX_BUF[33];
		Save_Data_LCD[14] = USART1_RX_BUF[37];
		Save_Data_LCD[15] = ((USART1_RX_BUF[41]&0x0F)<<4)|(USART1_RX_BUF[45]&0x0F);
		Save_Data_LCD[16] = USART1_RX_BUF[49];//Ԥ��7*8
		Save_Data_LCD[17] = USART1_RX_BUF[50];
		Save_Data_LCD[18] = USART1_RX_BUF[51];
		Save_Data_LCD[19] = USART1_RX_BUF[52];
		Save_Data_LCD[20] = USART1_RX_BUF[53];
		Save_Data_LCD[21] = USART1_RX_BUF[54];
		Save_Data_LCD[22] = USART1_RX_BUF[55];
		
		for (j = 1; j < 23; j++)
		{
			temp ^= Save_Data_LCD[j];
		}
		
		Save_Data_LCD[23] = temp;			//У����
		Save_Data_LCD[24] = USART1_RX_BUF[57];//֡β	
		
		USART1_RX_STA1=0xFF00;
	}
}

void SAVE_DATA(void)//PC��λ��
{
	if( (USART3_RX_STA==0xFF00)&&(Save_Data[24]==0xAA) )//�������ݴ��� 
	{
		STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Save_Data,SIZE);//д��STM32
		USART3_RX_STA=0x00FF;
	}
	
	if( (USART1_RX_STA1==0xFF00)&&(Save_Data_LCD[24]==0xAA) )//Һ�����ݴ���  
	{
		STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Save_Data_LCD,SIZE);//д��STM32
		USART1_RX_STA1=0x00FF;
	}
}

void READ_DATA(void)//��������
{
	u8 Fr_temp;
	u8 i;
	char s[10];
	float temp_Atten;//˥��Attendb
	
	if( (USART3_RX_STA==0x00FF)&&(Save_Data[24]==0xAA) )//��������д��CPLD
	{
		STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE); //�������ݵ�������
		
		for(i=0;i<25;i++)
			SPI_Flash_Write(datatemp[i], REG_ADDR+i);//дCPLD����
		
		Fr_temp = (datatemp[15]&0xF0)>>4;
		Fre_Seting(Fr_temp);
		
		USART3_RX_STA=0x0FFF;
	}
	/////////////////////////////////////////////////////
	if( (USART1_RX_STA1==0x00FF)&&(Save_Data_LCD[24]==0xAA) )//Һ������д��CPLD
	{
		STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE); //�������ݵ�������
		
		for(i=0;i<25;i++)
			SPI_Flash_Write(datatemp[i], REG_ADDR+i);//дCPLD����
		
		Fr_temp = (datatemp[15]&0xF0)>>4;
		Fre_Seting(Fr_temp);
		
		temp_Atten = SPI_Flash_Read(REG_ADDR_READ+13)+SPI_Flash_Read(REG_ADDR_READ+14)/10.0;
		Atten_Seting(temp_Atten);
		
		USART1_RX_STA1=0x0FFF;
	}
	
	if((USART1_RX_STA1==0x0FFF||USART3_RX_STA==0x0FFF)&&(Save_Data[24]==0xAA||Save_Data_LCD[24]==0xAA))
	{
		SHOW_DATA();
		USART1_RX_STA1=0xFFFF;
		USART3_RX_STA=0xFFFF;
	}
	/************************������֤******************************/
	if((USART1_RX_STA1==0xFFFF||USART3_RX_STA==0xFFFF)&&(Save_Data[24]==0xAA||Save_Data_LCD[24]==0xAA))
	{
		USART_SendData(USART3, 0x80);//���óɹ�
		sprintf(s,"t29.txt=\"���óɹ�\"");
		HMISends(s);
		USART1_RX_STA1=0;
		USART3_RX_STA=0;
	}
	else if((USART1_RX_STA1==0xFFFF||USART3_RX_STA==0xFFFF)&&(Save_Data[24]!=0xAA||Save_Data_LCD[24]!=0xAA))
	{
		USART_SendData(USART3, 0x08);//����ʧ��
		sprintf(s,"t29.txt=\"����ʧ��\"");
		HMISends(s);
		USART1_RX_STA1=0;
		USART3_RX_STA=0;
	}
}

void DATA_INIT(void)
{
	u8 i;
	u8 Fr_temp;
	
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,SIZE); //�������ݵ�������
	for(i=0;i<25;i++)
		SPI_Flash_Write(datatemp[i], REG_ADDR+i);//дCPLD����

	Fr_temp = (SPI_Flash_Read(REG_ADDR_READ+15)&0xF0)>>4;
	Fre_Seting(Fr_temp);
		
	 C0_5 = 1;	//��С����
	 C1_0 = 1;  
	 C2_0 = 1;  
	 C4_0 = 1; 
	 C8_0 = 1;  
	 C16_0= 1;	 
}

void SHOW_DATA(void)
{
	u8 x,y,z;
	u16 X,Y,Z;
	u16 clock_err,power1,power2;
	u8 PRN1,PRN2,PRN;
	u8 ID;
	
	char buffer_X[1],buffer_Y[1],buffer_Z[1];
	char buffer_x[1],buffer_y[1],buffer_z[1];
	char buffer_Clocl_Err[1];
	char buffer_Power1[1],buffer_Power2[1];
	char buffer_PRN1[1],buffer_PRN2[1];
	char buffer_ID[1];
	
	/************************������ʾ***************************/
	ID = SPI_Flash_Read(REG_ADDR_READ+1);
	sprintf(buffer_ID,"n12.val=%d",ID);
	HMISends(buffer_ID);
	
	X = (((u16)SPI_Flash_Read( REG_ADDR_READ+2 ))<<8)|SPI_Flash_Read(REG_ADDR_READ+3);//0--65535
	sprintf(buffer_X,"n13.val=%d",X);		
	HMISends(buffer_X);
	x = SPI_Flash_Read( REG_ADDR_READ+4 );																						//0--255С�������λ
	sprintf(buffer_x,"n14.val=%d",x);
	HMISends(buffer_x);
	
	Y = (((u16)SPI_Flash_Read( REG_ADDR_READ+5 ))<<8)|SPI_Flash_Read(REG_ADDR_READ+6);
	sprintf(buffer_Y,"n15.val=%d",Y);
	HMISends(buffer_Y);
	y = SPI_Flash_Read( REG_ADDR_READ+7 );
	sprintf(buffer_y,"n16.val=%d",y);
	HMISends(buffer_y);
	
	Z = (((u16)SPI_Flash_Read( REG_ADDR_READ+8 ))<<8)|SPI_Flash_Read(REG_ADDR_READ+9);
	sprintf(buffer_Z,"n17.val=%d",Z);
	HMISends(buffer_Z);
	z = SPI_Flash_Read( REG_ADDR_READ+10 );
	sprintf(buffer_z,"n18.val=%d",z);
	HMISends(buffer_z);
	
	clock_err = (((u16)SPI_Flash_Read( REG_ADDR_READ+11 ))<<8)|SPI_Flash_Read(REG_ADDR_READ+12);
	sprintf(buffer_Clocl_Err,"n19.val=%d",clock_err);
	HMISends(buffer_Clocl_Err);
	
	power1 = SPI_Flash_Read( REG_ADDR_READ+13 );
	sprintf(buffer_Power1,"n20.val=%d",power1);
	HMISends(buffer_Power1);
	power2 = SPI_Flash_Read( REG_ADDR_READ+14 );
	sprintf(buffer_Power2,"n21.val=%d",power2);
	HMISends(buffer_Power2);

	PRN = SPI_Flash_Read(REG_ADDR_READ+15);
	PRN1=(PRN&0xF0)>>4;
	sprintf(buffer_PRN1,"n22.val=%d",PRN1);//�ŵ�
	HMISends(buffer_PRN1);
	PRN2=PRN&0x0F;
	sprintf(buffer_PRN2,"n23.val=%d",PRN2);//PRN
	HMISends(buffer_PRN2);
}

void Fre_Seting(u8 Fr_temp)//Ƶ������
{
		Write_Amplitude(1);
	
		if(Fr_temp==1)				//����ƴƵ��
		{						
			WriteToADF435X(0x17EA3F8);//907.401 + 50 = 957.401
		  WriteToADF435X(0x800A711);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);
			
			WriteToADF435X(0x17EA3F8);
		  WriteToADF435X(0x800A711);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);
			
			R4_V = 0xA2803C;
			Write_Amplitude(0);
		}
		else if(Fr_temp==2)
		{
			WriteToADF435X(0x1800178);//910.470 +50 = 960.470
		  WriteToADF435X(0x80083E9);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);

			WriteToADF435X(0x1800178);
		  WriteToADF435X(0x80083E9);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);
			
			R4_V = 0xA2803C;
			Write_Amplitude(0);
		}
		else if(Fr_temp==3)
		{			
			WriteToADF435X(0x1812078);//913.539 + 50 = 963.539
		  WriteToADF435X(0x800A711);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);

			WriteToADF435X(0x1812078);
		  WriteToADF435X(0x800A711);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);
			
			R4_V = 0xA2803C;
			Write_Amplitude(0);
		}
		else if(Fr_temp==4)
		{			
			WriteToADF435X(0x1828598);//916.608 + 50 = 966.608
		  WriteToADF435X(0x8009389);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);

			WriteToADF435X(0x1828598);
		  WriteToADF435X(0x8009389);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);
			
			R4_V = 0xA2803C;
			Write_Amplitude(0);
		}
		else if(Fr_temp==5)
		{
			WriteToADF435X(0x1839CF8);//919.677 + 50 = 969.677
		  WriteToADF435X(0x800A711);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);

			WriteToADF435X(0x1839CF8);
		  WriteToADF435X(0x800A711);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);
			
			R4_V = 0xA2803C;
			Write_Amplitude(0);
		}
		else if(Fr_temp==6)
		{
			WriteToADF435X(0x18503D8);//922.746 + 50 = 972.746
		  WriteToADF435X(0x8009389);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);

			WriteToADF435X(0x18503D8);
		  WriteToADF435X(0x8009389);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);
			
			R4_V = 0xA2803C;
			Write_Amplitude(0);
		}
		else if(Fr_temp==0)
		{
			WriteToADF435X(0x1820150);//915.42 ��Ƶ
		  WriteToADF435X(0x80083E9);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);

			WriteToADF435X(0x1820150);
		  WriteToADF435X(0x80083E9);
		  WriteToADF435X(0x1004E42);
		  WriteToADF435X(0x4B3);
		  WriteToADF435X(0xA2803C);
		  WriteToADF435X(0x580005);
			
			R4_V = 0xA2803C;
			Write_Amplitude(0);
		}
}

//No1 = 25
//No2 = 29 -----
//No3 = 26
//No4 = 28
//No5 = 27.5

void Atten_Seting(float num)
{
	float Atten;
	
	Atten = 27.5 - num;
	
	if(Atten<=0)
		Atten = 0;
	Set_Voltage_Gain(Atten);
	
}
