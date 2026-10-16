/*
    bit 0 = right(RD5)
    bit 1 = center(RD6)
    bit 2 = left (RD7)
    active low --> 0 = black 
*/

#ifndef IR_SENSOR_H
#define	IR_SENSOR_H

#include <stdint.h>

#define IR_PATTERN_CENTER       2 // 010  only middle on line
#define IR_PATTERN_SLIGHT_RIGHT 3 // 011  middle + right
#define IR_PATTERN_SLIGHT_LEFT  6 // 110  left + middle
#define IR_PATTERN_RIGHT        1 // 001  only right
#define IR_PATTERN_LEFT         4 // 100  only left
#define IR_PATTERN_ALL_BLACK    7 // 111  crossing / stop mark
#define IR_PATTERN_LOST         0 // 000  all white

void IR_Init(void);
uint8_t IR_Read(void);

#endif	/* IR_SENSOR_H */
