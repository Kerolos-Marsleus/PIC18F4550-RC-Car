#ifndef SERVO_H
#define	SERVO_H
#include <stdint.h>


void Servo_Init(void);
void Servo_Handler(void);
void Servo_SetPulseUs(uint16_t pulse_us);
#endif /*SERVO_H*/