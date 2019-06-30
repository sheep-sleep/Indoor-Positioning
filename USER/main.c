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

u8 SYS_INIT_OK = 0;  //ϵͳ��ʼ��

void SYS_INIT(void)
{
	 delay_init();	    	 		//��ʱ������ʼ��
	 LED_Init();			    		//LED�˿ڳ�ʼ��
	 Init_AD9954();
	 set_2psk(50000000,0,180); 
	 SPI_Flash_Init();  			//SPI��ʼ��   
	 NVIC_Configuration(); 		//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 uart1_init(115200);	 		//���ڳ�ʼ��Ϊ115200 ��ϵ��Һ����ͨ��
	 uart3_init(115200);			//��λ��ͨ��
	 TIM3_Int_Init(150,7199);	//10Khz�ļ���Ƶ�ʣ�10�ļ������ڣ�10ms�ж�
	 HMISendstart();          //Ϊȷ������HMI����ͨ��
	 DATA_INIT();             //��ʼ���ϴε��������
	 delay_ms(10);
	 HMISends("page init");
	 delay_ms(800);
	 HMISends("page trans");
	 delay_ms(10);
	 SHOW_DATA();							//��ʾ�ϴα��������
}

int main(void)
{
	 SYS_INIT_OK = 0;
	 SYS_INIT();
	 SYS_INIT_OK = 1;
	 while(1)
		{}
}

