#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include "stm32f10x.h"

void init_output(void);
void MosfetOutputOn(void);
void MosfetOutputOff(void);
void RelayOutputOn(void);
void RelayOutputOff(void);

#endif 
