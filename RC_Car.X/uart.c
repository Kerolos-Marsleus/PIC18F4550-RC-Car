#include <xc.h>
#include "uart.h"


void UART_Init(void){
    BAUDCON = 0x00;         // 8-bit BRG, no inversion
    TRISCbits.TRISC6 = 1;   // tx pin . EUSART takes control
    TRISCbits.TRISC7 = 1;
    SPBRG = 25;             // baud rate 9600 with 16Mhz crystal
    TXSTA = (1 << _TXSTA_TXEN_POSN); // tx enabled, async, 8-bit, low speed
    RCSTA = (1 <<_RCSTA_SPEN_POSN) | (1 << _RCSTA_CREN_POSN);
    PIE1bits.RCIE = 1;
}


void UART_SendByte(uint8_t data){
    while(PIR1bits.TXIF == 0);  // wait if there is data had not transmitted
    TXREG = data;           // PUT THE DATA ON THE REGISTER
}

uint8_t UART_ReceiveByte(void){
    if(RCSTAbits.OERR == 1){    // overrun: receiver locked
        RCSTAbits.CREN = 0;     // reset overrun by toggling cren
        RCSTAbits.CREN = 1;
    }
    return RCREG;
}

void UART_SendNumber(uint16_t number){
    uint8_t arr[5];
    uint8_t count = 0;
    do{
        uint8_t digit = number % 10;
        arr[count] = digit;
        number /= 10;
        count++;
    }while(number > 0);

    while(count > 0){
        count--;
        UART_SendByte(arr[count]+'0');
    }
    
}