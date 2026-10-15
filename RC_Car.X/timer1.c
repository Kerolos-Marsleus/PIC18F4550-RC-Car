#include "timer1.h"
#include <xc.h>

void Timer1_Init(void){
    T1CON = 0xB0;   // 0b10110000 prescale 1:8, and 16_bit mode and timer is off
}

void Timer1_Start(void){
    T1CONbits.TMR1ON = 1;
}

void Timer1_Stop(void){
    T1CONbits.TMR1ON = 0;
}

void Timer1_Reset(void){
    TMR1H = 0x00;
    TMR1L = 0x00;
}

uint16_t Timer1_Read(void){
    uint8_t low = TMR1L;
    uint16_t value = TMR1H;
    value <<= 8;
    value |= low;
    return value;
}