/************************************/
#include "stm32f10x_it.h" 
#include "led.h"
#include "HMI.h"
#include "data.h"
#include "usart.h"	
#include "ADF435X.h"	
extern u8 SYS_INIT_OK;
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
//	u8 i, len,t;
	static u8 ms1  = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		
		if(!SYS_INIT_OK)//ϵͳ��ʼ�����ɹ�
			return;
		ms1++;
		
		if(ms1==1)//1ms
		{			
			HMIRcv();//Һ�������ݽ��ղ���
		}
		if(ms1==2)
		{
			INTEGRATE_DATA();//У�����������
			SAVE_DATA();
			
		}
		if(ms1==3)
		{
			ms1=0;
			READ_DATA();
		}
	}
}


