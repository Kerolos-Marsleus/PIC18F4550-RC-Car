/*
 * File:   main.c
 * Author: No_One
 *
 * Created on October 10, 2026, 6:52 PM
 */


#include <xc.h>
#define _XTAL_FREQ 16000000UL

#define LED_DIR TRISDbits.TRISD0
#define LED     LATDbits.LATD0

#define RESET_LED_DIR   TRISAbits.RA0
#define RESET_LED       LATAbits.LA0
    
void main(void) {
    ADCON1 = 0x0F;
    RESET_LED_DIR = 0;
    RESET_LED = 1;
    __delay_ms(100);
    RESET_LED = 0;
    LED_DIR = 0;
    LED = 0;
    while(1){
        LED ^= 1;
        __delay_ms(200);
    }
}
