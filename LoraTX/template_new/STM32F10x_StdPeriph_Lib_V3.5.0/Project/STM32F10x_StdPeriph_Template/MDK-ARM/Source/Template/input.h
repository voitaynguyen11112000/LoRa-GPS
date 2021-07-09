#ifndef _INPUT_H_
#define _INPUT_H_

#include "stm32f10x.h"

extern uint16_t key_code[8];

void init_input(void);
void scan_input(void);

#endif 
