#include "flash.h" 
#include "spi.h"

#include "data.h"

void SPI_Flash_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE );//PORTB???? 
  //????
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
	SPI1_Init();		   	//???SPI
//	SPI1_SetSpeed(SPI_BaudRatePrescaler_64);
}
//???
u8 SPI_Flash_Read( u8 ReadAddr )
{
 	u8 pBuffer;
	SPI_FLASH_CS=0;
	SPI1_ReadWriteByte(ReadAddr);
	pBuffer = SPI1_ReadWriteByte(0XFF);
	SPI_FLASH_CS=1;  
	return pBuffer;
}
//???
void SPI_Flash_Write( u8 pBuffer, u8 WriteAddr )
{
	  SPI_FLASH_CS = 0;
    SPI1_ReadWriteByte(WriteAddr);
		SPI1_ReadWriteByte(pBuffer);
		SPI_FLASH_CS = 1;
		
		SPI_Flash_Wait_Busy();
}
//????
void SPI_Flash_Wait_Busy(void)
{
	while (( SPI_Flash_Read(ReadStatusAddr+24)&0x80)==0x80);   // ??BUSY???(????CPLD??24????)
}

//????16bit,???????
//u16 match_16bit( u8 datatemp1, u8 datatemp2)
//{
//	return (datatemp1<<8)|datatemp2;
//}

////????24bit,???????
//u32 match_24bit( u8 datatemp1, u8 datatemp2 ,u8 datatemp3)
//{
//	return (datatemp1<<16)|(datatemp2<<8)|datatemp3;
//}

