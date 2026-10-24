#include "timer0.h"
#include <xc.h>

#define TIMER0_PRELOAD_1MS  6

static volatile uint32_t ms_ticks = 0;

void Timer0_Init(void){
    T0CON = 0x43;           // prescale 16 timer0 is off, 8bit mode, internal clock
    TMR0L = TIMER0_PRELOAD_1MS;
    INTCONbits.TMR0IF = 0;
    INTCONbits.TMR0IE = 1;
    T0CONbits.TMR0ON = 1;
}

uint32_t Timer0_getMs(void){
    // should protect the copy of ms_ticks
    INTCONbits.TMR0IE = 0;
    uint32_t ticks =  ms_ticks;
    INTCONbits.TMR0IE = 1;
    return ticks;
}

void Timer0_Handler(void){
    TMR0L = TIMER0_PRELOAD_1MS;
    INTCONbits.TMR0IF = 0;
    ms_ticks++;
}