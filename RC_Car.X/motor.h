#ifndef MOTOR_H
#define	MOTOR_H

#include <stdint.h>
//  RD3=IN4 RD2=IN3 RD1=IN2 RD0=IN1
#define MOTOR_FWD       0b0101
#define MOTOR_BWD       0b1010
#define MOTOR_LEFT      0b0100
#define MOTOR_RIGHT     0b0001
#define MOTOR_STOP      0b0000
#define MOTOR_SPIN_LEFT 0b0110
#define MOTOR_SPIN_RIGHT 0b1001

void Motor_Init(void);
void Motor_SetDirection(uint8_t pattern);

#endif	/* MOTOR_H */
