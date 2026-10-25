#include "servo.h"
#include <xc.h>

#define SERVO_COUNTS_PER_US     2U
#define SERVO_PERIOD_US         20000U


static volatile uint16_t servo_pulse_us = 1500;

static void Servo_LoadTime(uint16_t duration_us){
    uint16_t count = duration_us * SERVO_COUNTS_PER_US;
    uint16_t start_count = 65536 - count;
    TMR3H = start_count >> 8;
    TMR3L = start_count; 
}
void Servo_Init(void){
    LATBbits.LATB1 = 0;
    TRISBbits.TRISB1 = 0;
    T3CON = 0b10010000;
    Servo_LoadTime(SERVO_PERIOD_US - servo_pulse_us);

    PIR2bits.TMR3IF = 0;
    PIE2bits.TMR3IE = 1;
    T3CONbits.TMR3ON = 1;
}

