#ifndef __HMI_H
#define __HMI_H 			   
#include "sys.h"

void HMISends(char *buf1);	//�ַ������ͺ���
void HMISendb(u8 buf);			//�ֽڷ��ͺ���
void HMISendstart(void);		//Ϊȷ������HMI����ͨ��
void HMIRcv(void);					//���պ���

#endif
