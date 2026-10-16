/*
 * File:   main.c
 * Author: No_One
 *
 * Created on October 10, 2026, 8:55 PM
 */

 
#include <xc.h>
#include "configure.h"
#include "uart.h"
#include "motor.h"
#include "pwm.h"
#include "ultrasonic.h"
#include "obstacle.h"
#include "line_follower.h"
#include "ir_sensor.h"


#define RESET_LED_DIR   TRISAbits.TRISA0
#define RESET_LED       LATAbits.LATA0
#define MODE_MANUAL     1
#define MODE_OBSTACLE   2
#define MODE_LINE       3
#define MODE_MAZE       4


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
    Ultrasonic_Init();
    IR_Init();
    Motor_SetSpeed(0, 0);
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    uint8_t mode = MODE_MANUAL;
    
    while(1){
        if(rx_flag == 1){
            rx_flag = 0;
            uint8_t command = rx_data;
            switch (command) {
                case 'q':
                    Motor_SetSpeed(100, 100);
                    break;
                // motor mode setup
                case 'U':
                    Motor_SetDirection(MOTOR_STOP);
                    mode = MODE_OBSTACLE;
                    break;
                case 'V':
                    Motor_SetDirection(MOTOR_STOP);
                    mode = MODE_LINE;
                    break;
                case 'X':
                    Motor_SetDirection(MOTOR_STOP);
                    mode = MODE_MAZE;
                    break;
                case 'w':
                case 'u':
                case 'v':
                case 'x':
                case 'D':
                case 'W':
                    Motor_SetDirection(MOTOR_STOP);
                    mode = MODE_MANUAL;
                    break;
                default:
                    break;
            }
            if(mode == MODE_MANUAL){

                switch (command){
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
                    default:
                        break;
                }
            }
            if(command >= '0' && command <= '9'){
                uint8_t speed = (command - '0') * 10;
                Motor_SetSpeed(speed, speed);
            }
        }
        switch (mode) {
            case MODE_OBSTACLE:
                Obstacle_Run();
                break;
            case MODE_LINE:
                Line_Run();
                break;
            default:
                break;
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
