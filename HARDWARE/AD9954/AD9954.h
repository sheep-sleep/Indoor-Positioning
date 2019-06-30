#ifndef __AD9954_H
#define __AD9954_H	 
#include "sys.h"

void Init_AD9954(void);  //INIT 9954 
void send_data(u8 dat );

void set_2psk(float f1,float phase1,float phase2);
void set_freq(float f);
#endif

