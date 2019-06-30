#ifndef __DATA_H
#define __DATA_H			    
#include "sys.h" 

extern u8 START_CODE;   //ͬ����
extern u8 FINISH_CODE;  //������
extern u8 REG_ADDR;//д�Ĵ����׵�ַ
extern u8 REG_ADDR_READ;//���Ĵ����׵�ַ
extern u8 ReadStatusAddr;
extern u8 Save_Data[25];
extern u8 Save_Data_LCD[25];
extern u8 datatemp[25];//���ݻ�����
//u8 EVEN_8bit(u8 Data);
u8 DATA_EVEN_RS232(u8 *buffer);
u8 DATA_EVEN_LCD(u8 *buffer);
void INTEGRATE_DATA(void);
void SAVE_DATA(void);
void READ_DATA(void);
void DATA_INIT(void);
void SHOW_DATA(void);
void Fre_Seting(u8 Fr_temp);
void Atten_Seting(float num);
#endif
