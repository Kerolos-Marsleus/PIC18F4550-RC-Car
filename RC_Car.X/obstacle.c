
#include "obstacle.h"
#include "motor.h"
#include "ultrasonic.h"
#include "configure.h"
#include <xc.h>

#define OBSTACLE_LIMIT_CM   30
#define REST_PERIOD_MS      60
#define DEFAULT_SPIN_DIRECTION MOTOR_SPIN_RIGHT

void Obstacle_Run(void){
    uint16_t distance = Ultrasonic_GetDistance();
    if(distance == US_NO_ECHO){
        Motor_SetDirection(MOTOR_STOP);
    }else if (distance < OBSTACLE_LIMIT_CM){
        Motor_SetDirection(DEFAULT_SPIN_DIRECTION);
    }else{
        Motor_SetDirection(MOTOR_FWD);
    }
    __delay_ms(REST_PERIOD_MS);
}