#include "configure.h"
#include "ultrasonic.h"
#include "timer1.h"
#include <xc.h>

#define US_TRIG             LATDbits.LATD4
#define US_TRIG_DIR         TRISDbits.TRISD4
#define US_ECHO             PORTBbits.RB0
#define US_ECHO_DIR         TRISBbits.TRISB0

void Ultrasonic_Init(void){
    US_TRIG = 0;
    US_TRIG_DIR = 0;
    US_ECHO_DIR = 1;
    Timer1_Init();
}

uint16_t Ultrasonic_GetDistance(void){
    US_TRIG = 1;
    __delay_us(10);
    US_TRIG = 0;
    
    Timer1_Reset();
    PIR1bits.TMR1IF = 0;
    Timer1_Start();
    while(PIR1bits.TMR1IF == 0 && US_ECHO == 0); // wait for the responce
    if(PIR1bits.TMR1IF == 1){
        Timer1_Stop();
        return US_NO_ECHO;
    }
    // measure the time of the responce
    Timer1_Reset();
    while( PIR1bits.TMR1IF == 0 && US_ECHO == 1);
    if(PIR1bits.TMR1IF == 1){
        Timer1_Stop();
        return US_NO_ECHO;
    }
    Timer1_Stop();
    uint16_t ticks = Timer1_Read();
    return ticks / 29;

}

