#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
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


void LED_Init(void);//初始化

		 				    
#endif
