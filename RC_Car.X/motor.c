#include <xc.h>
#include "motor.h"

void Motor_Init(void){
    // from RD0-RD3 output and en at full speed now
    Motor_SetDirection(MOTOR_STOP);
    TRISD &= 0xF0;
    LATCbits.LATC1 = 1;
    LATCbits.LATC2 = 1;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;

}
void Motor_SetDirection(uint8_t pattern){
    LATD = (LATD & 0xF0) | (pattern & 0x0F);
}