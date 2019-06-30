#include "sys.h"
#include "led.h"
#include "delay.h"
#include "flash.h"
#include "usart.h"
#include "HMI.h"
#include "timer.h"
#include "ADF435X.h"
#include "AD9954.h"
#include "stmflash.h"
#include "data.h"

u8 SYS_INIT_OK = 0;  //系统初始化

void SYS_INIT(void)
{
	 delay_init();	    	 		//延时函数初始化
	 LED_Init();			    		//LED端口初始化
	 Init_AD9954();
	 set_2psk(50000000,0,180); 
	 SPI_Flash_Init();  			//SPI初始化   
	 NVIC_Configuration(); 		//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	 uart1_init(115200);	 		//串口初始化为115200 关系到液晶屏通信
	 uart3_init(115200);			//上位机通信
	 TIM3_Int_Init(150,7199);	//10Khz的计数频率，10的计数周期，10ms中断
	 HMISendstart();          //为确保串口HMI正常通信
	 DATA_INIT();             //初始化上次掉电的数据
	 delay_ms(10);
	 HMISends("page init");
	 delay_ms(800);
	 HMISends("page trans");
	 delay_ms(10);
	 SHOW_DATA();							//显示上次保存的数据
}

int main(void)
{
	 SYS_INIT_OK = 0;
	 SYS_INIT();
	 SYS_INIT_OK = 1;
	 while(1)
		{}
}

