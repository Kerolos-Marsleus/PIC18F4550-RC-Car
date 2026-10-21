#include "line_follower.h"
#include "ir_sensor.h"
#include "motor.h"

#define LAST_STATUS_LEFT    0
#define LAST_STATUS_RIGHT   1

void Line_Run(uint8_t speed){
    uint8_t value = IR_Read();
    Line_Handler(value, speed);
    
}

void Line_Handler(uint8_t value,uint8_t speed){
    static uint8_t last_status = LAST_STATUS_LEFT;
    uint8_t slower_speed = Motor_TurnSpeed(speed);
    switch (value) {
        case IR_PATTERN_CENTER:
            Motor_SetSpeed(speed, speed);
            Motor_SetDirection(MOTOR_FWD);
            break;
        case IR_PATTERN_LEFT:
            Motor_SetSpeed(slower_speed, slower_speed);
            Motor_SetDirection(MOTOR_LEFT);
            last_status = LAST_STATUS_LEFT;
            break;
        case IR_PATTERN_RIGHT:
            Motor_SetSpeed(slower_speed, slower_speed);
            Motor_SetDirection(MOTOR_RIGHT);
            last_status = LAST_STATUS_RIGHT;
            break;
        case IR_PATTERN_SLIGHT_LEFT:
            Motor_SetSpeed(slower_speed, speed);
            Motor_SetDirection(MOTOR_FWD);
            last_status = LAST_STATUS_LEFT;
            break;
        case IR_PATTERN_SLIGHT_RIGHT:
            Motor_SetSpeed(speed, slower_speed);
            Motor_SetDirection(MOTOR_FWD);
            last_status = LAST_STATUS_RIGHT;
            break;
        case IR_PATTERN_ALL_BLACK:
            Motor_SetDirection(MOTOR_STOP);
            break;
        case IR_PATTERN_LOST:
            Motor_SetSpeed(slower_speed, slower_speed);
            if(last_status == LAST_STATUS_LEFT){
                Motor_SetDirection(MOTOR_SPIN_LEFT);
            }else{
                Motor_SetDirection(MOTOR_SPIN_RIGHT);
            }
            break;
        default:
            break;
    }
}