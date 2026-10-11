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

// shared between ISR and main
volatile uint8_t rx_data;
volatile uint8_t rx_flag;
void main(void) {
    
    ADCON1 = 0x0F;          // ALL PINS ARE DIGITAL
    RESET_LED = 1;
    RESET_LED_DIR = 0;
    __delay_ms(100);
    RESET_LED = 0;
    
    UART_Init();
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
    while(1){
        if(rx_flag == 1){
            rx_flag = 0;
            UART_SendByte(rx_data);
        }
        __delay_ms(500);
    }
}

void __interrupt() ISR(void)
{   
    if(PIR1bits.RCIF == 1){
        rx_data = UART_ReceiveByte();
        rx_flag = 1;
        
    }
}
