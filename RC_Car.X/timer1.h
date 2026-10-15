/*
    Timer1_init : prescale 1 : 8, tick 2usec @ 16Mhz, max 131 ms
*/
#ifndef TIMER1_H
#define	TIMER1_H
#include <stdint.h>


void Timer1_Init(void);
void Timer1_Start(void);
void Timer1_Stop(void);
void Timer1_Reset(void);
uint16_t Timer1_Read(void);

#endif	/* TIMER1_H */