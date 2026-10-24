#ifndef TIMER0_H
#define TIMER0_H
#include <stdint.h>



void Timer0_Init(void);
uint32_t Timer0_getMs(void);
void Timer0_Handler(void);


#endif  /*TIMER0_H*/