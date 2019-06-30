#ifndef __HMI_H
#define __HMI_H 			   
#include "sys.h"

void HMISends(char *buf1);	//字符串发送函数
void HMISendb(u8 buf);			//字节发送函数
void HMISendstart(void);		//为确保串口HMI正常通信
void HMIRcv(void);					//接收函数

#endif
