/************************************/
#include "stm32f10x_it.h" 
#include "led.h"
#include "HMI.h"
#include "data.h"
#include "usart.h"	
#include "ADF435X.h"	
extern u8 SYS_INIT_OK;
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
//	u8 i, len,t;
	static u8 ms1  = 0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		
		if(!SYS_INIT_OK)//系统初始化不成功
			return;
		ms1++;
		
		if(ms1==1)//1ms
		{			
			HMIRcv();//液晶屏数据接收操作
		}
		if(ms1==2)
		{
			INTEGRATE_DATA();//校验后整合数据
			SAVE_DATA();
			
		}
		if(ms1==3)
		{
			ms1=0;
			READ_DATA();
		}
	}
}


