/*
 * File:   main.c
 * Author: No_One
 *
 * Created on October 10, 2026, 8:55 PM
 */

 
#include <xc.h>
#include "uart.h"
#include "motor.h"
#include "pwm.h"

#define _XTAL_FREQ 16000000UL

#define RESET_LED_DIR   TRISAbits.TRISA0
#define RESET_LED       LATAbits.LATA0

// shared between ISR and main
volatile uint8_t rx_data;
volatile uint8_t rx_flag;
void main(void) {
    
    ADCON1 = 0x0F;          // ALL PINS ARE DIGITAL
    RESET_LED = 1;
    RESET_LED_DIR = 0;
    __delay_ms(100);
    RESET_LED = 0;
    
    UART_Init();
    PWM_Init();
    Motor_Init();
    Motor_SetSpeed(0, 0);
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    while(1){
        if(rx_flag == 1){
            rx_flag = 0;
            uint8_t comand = rx_data;
            switch (comand) {
                case 'F':
                    Motor_SetDirection(MOTOR_FWD);
                    break;
                case 'B':
                    Motor_SetDirection(MOTOR_BWD);
                    break;
                case 'L':
                    Motor_SetDirection(MOTOR_LEFT);
                    break;
                case 'R':
                    Motor_SetDirection(MOTOR_RIGHT);
                    break;
                case 'S':
                    Motor_SetDirection(MOTOR_STOP);
                    break;
                case 'q':
                    Motor_SetSpeed(100, 100);
                    break;
                default:
                    break;
            }
            if(comand >= '0' && comand <= '9'){
                uint8_t speed = (comand - '0') * 10;
                Motor_SetSpeed(speed, speed);
            }
        }
    }
}

void __interrupt() ISR(void)
{   
    if(PIR1bits.RCIF == 1){
        rx_data = UART_ReceiveByte();
        rx_flag = 1;
        
    }
}
