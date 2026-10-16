#include "line_follower.h"
#include "ir_sensor.h"
#include "motor.h"

#define LAST_STATUS_LEFT    0
#define LAST_STATUS_RIGHT   1


void Line_Run(void){
    uint8_t value = IR_Read();
    static uint8_t last_status = LAST_STATUS_LEFT;
    switch (value) {
        case IR_PATTERN_CENTER:
            Motor_SetDirection(MOTOR_FWD);
            break;
        case IR_PATTERN_LEFT:
            Motor_SetDirection(MOTOR_SPIN_LEFT);
            last_status = LAST_STATUS_LEFT;
            break;
        case IR_PATTERN_RIGHT:
            Motor_SetDirection(MOTOR_SPIN_RIGHT);
            last_status = LAST_STATUS_RIGHT;
            break;
        case IR_PATTERN_SLIGHT_LEFT:
            Motor_SetDirection(MOTOR_LEFT);
            last_status = LAST_STATUS_LEFT;
            break;
        case IR_PATTERN_SLIGHT_RIGHT:
            Motor_SetDirection(MOTOR_RIGHT);
            last_status = LAST_STATUS_RIGHT;
            break;
        case IR_PATTERN_ALL_BLACK:
            Motor_SetDirection(MOTOR_STOP);
            break;
        case IR_PATTERN_LOST:
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