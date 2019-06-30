#ifndef __FLASH_H
#define __FLASH_H			    
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//?????????,??????,??????????
//ALIENTEK??STM32???
//W25Q64 ??	   
//????@ALIENTEK
//????:www.openedv.com
//????:2012/9/9
//??:V1.0
//????,?????
//Copyright(C) ????????????? 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	 
#define	SPI_FLASH_CS PAout(4)  //CPLD_CS
//#define	SPI_4350_CS PBout(12)  //??CS
void SPI_Flash_Init(void);

u8 SPI_Flash_Read( u8 ReadAddr );   //??flash
void SPI_Flash_Write( u8 pBuffer, u8 WriteAddr );//??flash
//u16 match_16bit( u8 datatemp1, u8 datatemp2);
//u32 match_24bit( u8 datatemp1, u8 datatemp2 ,u8 datatemp3);
void SPI_Flash_Wait_Busy(void);    //????

#endif
















