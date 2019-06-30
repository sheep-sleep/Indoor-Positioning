 /*-----------------------------------------------
  ���ƣ�ADF4350 ����
  ���ݣ�
------------------------------------------------*/
#ifndef __ADF435X_H
#define __ADF435X_H
#include "sys.h"

#define uchar unsigned char
#define uint  unsigned int	
#define ulong  unsigned long int						

extern unsigned char key_input;
static unsigned long Frequence;	//Ƶ�ʣ���λKHZ   200000MHZ=860000
extern int  Amplitude[2];				//���ȣ�����ͨ��
extern unsigned int phase;			//��λ���λ ��
//extern float Atten;//˥��Attendb
extern unsigned long R4_V;
void Init_ADF435X(void);  //INIT ADF435X
void WriteToADF435X(unsigned long BUF);
void Write_frequence(unsigned long Freq);
void Write_Amplitude(unsigned char Amp);
void Set_Voltage_Gain(float Gain);

#endif

