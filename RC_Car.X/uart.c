#include <xc.h>
#include "uart.h"


void UART_Init(void){
    BAUDCON = 0x00;         // 8-bit BRG, no inversion
    TRISCbits.TRISC6 = 1;   // tx pin . EUSART takes control
    TRISCbits.TRISC7 = 1;
    SPBRG = 25;             // baud rate 9600 with 16Mhz crystal
    TXSTA = (1 << _TXSTA_TXEN_POSN); // tx enabled, async, 8-bit, low speed
    RCSTA = (1 <<_RCSTA_SPEN_POSN) | (1 << _RCSTA_CREN_POSN);
}


void UART_SendByte(uint8_t data){
    while(PIR1bits.TXIF == 0);  // wait if there is data had not transmitted
    TXREG = data;           // PUT THE DATA ON THE REGISTER
}

uint8_t UART_ReceiveByte(void){
    while(PIR1bits.RCIF == 0);
    return RCREG;
}