/* 
 * File:   uart.h
 * Author: No_One
 *
 * Created on October 10, 2026, 9:01 PM
 */

#ifndef UART_H
#define	UART_H

#include <stdint.h>
// initialize uart module
void UART_Init(void);
// send one byte to uart module
void UART_SendByte(uint8_t data);
// recieve one byte 
uint8_t UART_ReceiveByte(void);

void UART_SendNumber(uint16_t number);
#endif	/* UART_H */

