#ifndef LINE_FOLLOWER_H
#define	LINE_FOLLOWER_H

#include <stdint.h>

void Line_Run(uint8_t speed);
void Line_Handler(uint8_t value, uint8_t speed);

#endif	/* LINE_FOLLOWER_H */
