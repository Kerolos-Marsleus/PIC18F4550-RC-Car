#include "ir_sensor.h"
#include <xc.h>

#define IR_SHIFT 5

void IR_Init(void){
    TRISD |= 0xE0;
}




uint8_t IR_Read(void){
    uint8_t value = PORTD >> IR_SHIFT;
    return value;
}