#ifndef PWM_H
#define	PWM_H
#include <stdint.h>

void PWM_Init(void);
void PWM1_SetDuty(uint8_t percent);
void PWM2_SetDuty(uint8_t percent);

#endif	/* PWM_H */
