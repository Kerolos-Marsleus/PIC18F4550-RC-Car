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
#include "maze.h"
#include "timer0.h"


#define RESET_LED_DIR           TRISAbits.TRISA0
#define RESET_LED               LATAbits.LATA0
#define TIMER_TEST_LED_DIR      TRISAbits.TRISA1
#define TIMER_TEST_LED          LATAbits.LATA1
#define MODE_MANUAL             1
#define MODE_OBSTACLE           2
#define MODE_LINE               3
#define MODE_MAZE               4
#define DEFAULT_START_SPEED     70
#define LED_BLINK_MS            500
// shared between ISR and main
volatile uint8_t rx_data;
volatile uint8_t rx_flag;
void main(void) {
    
    ADCON1 = 0x0F;          // ALL PINS ARE DIGITAL
    RESET_LED = 1;
    RESET_LED_DIR = 0;
    __delay_ms(100);
    RESET_LED = 0;
    TIMER_TEST_LED_DIR = 0;

    UART_Init();
    PWM_Init();
    Motor_Init();
    Ultrasonic_Init();
    IR_Init();
    Timer0_Init();
    Motor_Reset(DEFAULT_START_SPEED);
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    uint8_t mode = MODE_MANUAL;
    uint8_t speed = DEFAULT_START_SPEED;
    uint32_t timer0_timeStart = Timer0_getMs();
    while(1){
        if(rx_flag == 1){
            rx_flag = 0;
            uint8_t command = rx_data;
            switch (command) {
                case 'q':
                    Motor_SetSpeed(100, 100);
                    speed = 100;
                    break;
                // motor mode setup
                case 'U':
                    Motor_Reset(speed);
                    mode = MODE_OBSTACLE;
                    break;
                case 'V':
                    Motor_Reset(speed);
                    mode = MODE_LINE;
                    break;
                case 'X':
                    Motor_Reset(speed);
                    mode = MODE_MAZE;
                    break;
                case 'w':
                case 'u':
                case 'v':
                case 'x':
                case 'D':
                case 'W':
                    Motor_Reset(speed);
                    mode = MODE_MANUAL;
                    break;
                default:
                    break;
            }
            if(mode == MODE_MANUAL){
                uint8_t slower_speed = Motor_TurnSpeed(speed);
                switch (command){
                    case 'F':
                        Motor_SetSpeed(speed, speed);
                        Motor_SetDirection(MOTOR_FWD);
                        break;
                    case 'B':
                        Motor_SetSpeed(speed, speed);
                        Motor_SetDirection(MOTOR_BWD);
                        break;
                    case 'L':
                        Motor_SetSpeed(speed, speed);
                        Motor_SetDirection(MOTOR_LEFT);
                        break;
                    case 'R':
                        Motor_SetSpeed(speed, speed);
                        Motor_SetDirection(MOTOR_RIGHT);
                        break;
                    case 'G':
                        Motor_SetSpeed(slower_speed, speed);
                        Motor_SetDirection(MOTOR_FWD);
                        break;
                    case 'I':
                        Motor_SetSpeed(speed, slower_speed);
                        Motor_SetDirection(MOTOR_FWD);
                        break;
                    case 'H':
                        Motor_SetSpeed(slower_speed, speed);
                        Motor_SetDirection(MOTOR_BWD);
                        break;
                    case 'J':
                        Motor_SetSpeed(speed, slower_speed);
                        Motor_SetDirection(MOTOR_BWD);
                        break;
                    case 'S':
                        Motor_SetDirection(MOTOR_STOP);
                        break;
                    default:
                        break;
                }
            }
            if(command >= '0' && command <= '9'){
                speed = (command - '0') * 10;
                Motor_SetSpeed(speed, speed);
            }
        }
        switch (mode) {
            case MODE_OBSTACLE:
                Obstacle_Run();
                break;
            case MODE_LINE:
                Line_Run(speed);
                break;
            case MODE_MAZE:
                Maze_Run(speed);
                break;
            default:
                break;
        }
        uint32_t timer0_timerEnd = Timer0_getMs();
        if(timer0_timerEnd-timer0_timeStart >= LED_BLINK_MS){
            TIMER_TEST_LED ^= 1;
            timer0_timeStart += LED_BLINK_MS;
        }
        
    }
}

void __interrupt() ISR(void)
{   
    if(PIR1bits.RCIF && PIE1bits.RCIE){
        rx_data = UART_ReceiveByte();
        rx_flag = 1;
        
    }
    if(INTCONbits.TMR0IF && INTCONbits.TMR0IE){
        Timer0_Handler();
    }
}
