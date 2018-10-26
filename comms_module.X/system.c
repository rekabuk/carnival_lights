/* 
 * File:   system.c
 * Author: Andrew Baker
 *
 * Created on 20 October 2018, 17:24
 */

#include <xc.h>
#include <stdint.h>

#include "system.h"
#include "user.h"

void Initialise( void)
{
    // Port A
    // Disable comparator 
    CMCONbits.CM = 7;
    
    TRISAbits.TRISA2 = 1;   // RXD - in
    
    // Port C bits    
    //TRISCbits.TRISC0 = 1;   // RXD - in
    TRISCbits.TRISC1 = 0;   // TXE - out
    TRISCbits.TRISC2 = 0;   // TXD - out
    TRISCbits.TRISC3 = 0;   // Test
    TRISCbits.TRISC4 = 0;   // Test
    TRISCbits.TRISC5 = 0;   // Test
    
    // Initialise TMR1 (Timer Tick))
    T1CONbits.TMR1CS = 0;   // Use Fosc/4
    T1CONbits.T1CKPS = 0;   // Pre-scaler = 1
    T1CONbits.nT1SYNC = 1;  // No sync required
    T1CONbits.TMR1GE = 0;   // TMR1 Run
    TMR1H = TMR1H_VAL;
    TMR1L = TMR1L_VAL;     
    PIR1bits.TMR1IF = 0;    // Clear interrupt
    PIE1bits.TMR1IE = 1;    // Enable PIE interrupt
    T1CONbits.TMR1ON = 1;   

    // TMR0 is for bit timing, use Fosc/4 and no prescaler
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 1;

    // Enable change of state detection on 
    IOCAbits.IOCA2 = 1;

    // Enable interrupt on RDX bit low-to high
    BitDataInit( RX_MODE);    
}

