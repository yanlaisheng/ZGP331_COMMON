#ifndef __TIM_H
#define __TIM_H

#include "stm32f10x.h"


extern uint8_t heart_beat_flag;        // ������־


void tim2_init(void);
void tim2_interrupt_process(void);


#endif





