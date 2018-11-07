/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"

void Configure(void)
{
    // TIMER 0 - Used to keep track of time
    // Input clock is 20E6, set timer 0 to produce interrupt
    OPTION_REGbits.T0CS = 0;           // Timer increments on instruction clock
    OPTION_REGbits.PSA = 0;            // Use Pre-scaler with instruction clock
    OPTION_REGbits.PS = 7;             // Pre-scaler = 256
    TMR0 = 61;                         // Give 9.984ms interrupt
    INTCONbits.T0IE = 1;               // Enable interrupt on TMR0 overflow
    
    // TIMER 1 - Used for bit timing
    // Transmit mode = 20us, Rx Mode = 30us
    T2CONbits.T2CKPS = 0;              // Sys clock (Fosc/4))
    T2CONbits.TOUTPS = 0;              // Output pos-scalar = 1:1
    PR2 = 100;                         // 20E6/4/100=50KHz = 20us
    T2CONbits.TMR2ON = 1;
    
    // Enable TMR1 interrupt and Peripheral interrupts
    INTCONbits.PEIE = 1;
    PIE1bits.TMR2IE = 1;
          
    // Configure Port A as Digital IO
    ADCON1bits.PCFG = 0x6;
    
    // Setup output pin 2 RA0
    PORTAbits.RA0 = 0;                 // Set output low
    TRISAbits.TRISA0 = 0;              // Set as output
}
