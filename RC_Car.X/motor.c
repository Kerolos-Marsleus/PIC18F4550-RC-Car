#include <xc.h>
#include "motor.h"
#include "pwm.h"

void Motor_Init(void){
    // from RD0-RD3 output and en at full speed now
    Motor_SetDirection(MOTOR_STOP);
    TRISD &= 0xF0;
    Motor_SetSpeed(0, 0);

}
void Motor_SetDirection(uint8_t pattern){
    LATD = (LATD & 0xF0) | (pattern & 0x0F);
}

void Motor_SetSpeed(uint8_t left_speed, uint8_t right_speed){
    PWM1_SetDuty(left_speed);
    PWM2_SetDuty(right_speed);
}