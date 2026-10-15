#ifndef ULTRASONIC_H
#define	ULTRASONIC_H
#include <stdint.h>

#define US_NO_ECHO          0xFFFF


void Ultrasonic_Init(void);
uint16_t Ultrasonic_GetDistance(void);


#endif	/* ULTRASONIC_H */