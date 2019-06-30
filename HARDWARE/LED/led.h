#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define Trans_EN  PCout(4)
#define LED0 		  PCout(14)
#define IO 		    PBout(12)

#define LE 			  PCout(9)
#define DATA 		  PAout(8)
#define CLK 		  PAout(11)
#define LD			  PAout(12)

#define C0_5		  PCout(7)
#define C1_0		  PCout(8)
#define C2_0		  PCout(6)
#define C4_0		  PBout(15)
#define C8_0		  PBout(14)
#define C16_0     PBout(13)

#define IOUPDATE 		PCout(10)
#define PS1 				PCout(11)
#define PS0 				PCout(12)
#define AD9954_SDIO PBout(3)
#define AD9954_SCLK PBout(4)
#define AD9954_CS 	PBout(5)
//#define AD9954_SDO	PBin(6)
#define AD9954_RES 	PBout(7)


void LED_Init(void);//��ʼ��

		 				    
#endif
