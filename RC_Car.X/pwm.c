#include <xc.h>
#include "pwm.h"

void PWM_Init(void){
    PR2 = 199;                              // for frequncey 1250hz with a crystal 16mhz
    CCPR1L = 0;
    CCPR2L = 0;                             // initialize the duty to be 0 for both ccp1 and ccp2
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;                   // make RC1 and 2 output pins
    T2CON = 0x06;                           // prescale 16, timer on , postscaler = 0
    CCP1CON = (CCP1CON & 0xC0) | (0x0C);    // chose pwm in ccp1
    CCP2CON = (CCP2CON & 0xC0) | (0x0C);    // chose pwm in ccp2
}

void PWM1_SetDuty(uint8_t percent){
    if(percent > 100)percent = 100;
    if(percent < 0) percent = 0;
    CCPR1L = (percent << 1) ;
}

void PWM2_SetDuty(uint8_t percent){
    if(percent > 100)percent = 100;
    if(percent < 0) percent = 0;
    CCPR2L = (percent << 1) ;
}