/*
 * File:   main.c
 * Author: No_One
 *
 * Created on October 10, 2026, 8:55 PM
 */


#include <xc.h>
#include "uart.h"

#define _XTAL_FREQ 16000000UL

#define RESET_LED_DIR   TRISAbits.TRISA0
#define RESET_LED       LATAbits.LATA0

void main(void) {
    
    ADCON1 = 0x0F;          // ALL PINS ARE DIGITAL
    RESET_LED = 1;
    RESET_LED_DIR = 0;
    __delay_ms(100);
    RESET_LED = 0;
    
    UART_Init();
    while(1){
        uint8_t data = UART_ReceiveByte();
        UART_SendByte(data);
    }
}
