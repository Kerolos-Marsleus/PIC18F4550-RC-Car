#include "servo.h"
#include <xc.h>

#define SERVO_COUNTS_PER_US     2U
#define SERVO_PERIOD_US         20000U
#define SERVO_PIN_DATA          LATBbits.LATB1
#define SERVO_PIN_DIR           TRISBbits.TRISB1
#define SERVO_MIN_US            500U
#define SERVO_MAX_US            2500U
static volatile uint16_t servo_pulse_us = 1500;

static void Servo_LoadTime(uint16_t duration_us){
    uint16_t count = duration_us * SERVO_COUNTS_PER_US;
    uint16_t start_count = 65536 - count;
    TMR3H = start_count >> 8;
    TMR3L = start_count; 
}
void Servo_Init(void){
    SERVO_PIN_DATA = 0;
    SERVO_PIN_DIR = 0;
    T3CON = 0b10010000;
    Servo_LoadTime(SERVO_PERIOD_US - servo_pulse_us);

    PIR2bits.TMR3IF = 0;
    PIE2bits.TMR3IE = 1;
    T3CONbits.TMR3ON = 1;
}

void Servo_SetPulseUs(uint16_t pulse_us){
    if(pulse_us < SERVO_MIN_US){
        pulse_us = SERVO_MIN_US;
    }else if (pulse_us > SERVO_MAX_US){
        pulse_us = SERVO_MAX_US;
    }
    // servo_pulse_us is a 16 bit value so i need to protect it while WRITING it and because it is shared with the isr
    TMR3IE = 0;
    servo_pulse_us  = pulse_us;
    TMR3IE = 1;
}

void Servo_Handler(void){
    TMR3IF = 0;
    if(SERVO_PIN_DATA){
        SERVO_PIN_DATA = 0;
        Servo_LoadTime(SERVO_PERIOD_US - servo_pulse_us);
    }else if(SERVO_PIN_DATA == 0){
        SERVO_PIN_DATA = 1;
        Servo_LoadTime(servo_pulse_us);
    }
}
