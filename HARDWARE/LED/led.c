
#include "led.h"


void LED_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 //
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//AFIO时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);// 改变指定管脚的映射 GPIO_Remap_SWJ_Disable SWJ 完全禁用（JTAG+SW-DP）
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);// 改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable ，JTAG-DP 禁用 + SW-DP 使能

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//LED				 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
  GPIO_SetBits(GPIOC,GPIO_Pin_14);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//IO		 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
  GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//Trans_EN		
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			
  GPIO_Init(GPIOC, &GPIO_InitStructure);					
  GPIO_ResetBits(GPIOC,GPIO_Pin_4);					
	/******************4350********************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//LE
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOC, &GPIO_InitStructure);					
  GPIO_SetBits(GPIOC,GPIO_Pin_9);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//DATA	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOA, &GPIO_InitStructure);					
  GPIO_SetBits(GPIOA,GPIO_Pin_8);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//CLK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOA, &GPIO_InitStructure);					
  GPIO_SetBits(GPIOA,GPIO_Pin_11);				
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//LD
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOA, &GPIO_InitStructure);					
  GPIO_SetBits(GPIOA,GPIO_Pin_12);			
	/******************4302******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//C0_5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOC, &GPIO_InitStructure);					
  GPIO_SetBits(GPIOC,GPIO_Pin_8);				 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//C1_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOC, &GPIO_InitStructure);					
  GPIO_SetBits(GPIOC,GPIO_Pin_7);				 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//C2_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOC, &GPIO_InitStructure);					
  GPIO_SetBits(GPIOC,GPIO_Pin_6);				
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//C4_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOB, &GPIO_InitStructure);					
  GPIO_SetBits(GPIOB,GPIO_Pin_15);			

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//C8_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOB, &GPIO_InitStructure);					
  GPIO_SetBits(GPIOB,GPIO_Pin_14);		

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//C16_0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
  GPIO_Init(GPIOB, &GPIO_InitStructure);					
  GPIO_SetBits(GPIOB,GPIO_Pin_13);				
	/*******************9954********************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12; // UPDATA-->PS1-->PS0/CPLD_S
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_7; // SDIO-->SCLK-->CS-->RESET
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 
	GPIO_Init(GPIOB, &GPIO_InitStructure);				 
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//SDO    		 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
//	AD9954_IOSY=0;
//	AD9954_OSK=0;
//	AD9954_PWR=0;		
	
}
