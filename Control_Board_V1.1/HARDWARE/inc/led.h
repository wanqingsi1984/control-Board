#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED0 PDout(6)
#define LED1 PEout(7) 
#define BEEP_ON GPIO_SetBits(GPIOE,GPIO_Pin_11)
#define BEEP_OFF GPIO_ResetBits(GPIOE,GPIO_Pin_11)


void LED_BEEP_Init(void);//≥ı ºªØ		
void Beep_ms(u8 ms);
#endif
