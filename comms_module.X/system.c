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
    
    // Port C bits    
    TRISCbits.TRISC0 = 1;   // RXD - in
    TRISCbits.TRISC1 = 0;   // TXE - out
    TRISCbits.TRISC2 = 0;   // TXD - out
    TRISCbits.TRISC3 = 0;   // Test
    TRISCbits.TRISC4 = 0;   // Test
    TRISCbits.TRISC5 = 0;   // Test
    
    // Initialise TMR1
    T1CONbits.TMR1CS = 0;   // Use Fosc/4
    T1CONbits.T1CKPS = 0;   // Pre-scaler = 1
    T1CONbits.nT1SYNC = 1;  // No sync required
    T1CONbits.TMR1GE = 0;   // TMR1 Run
    
    // Keeps track of time
    StartTickTimer();

    // Enable interrupt on RDX bit low-to high
    BitDataInit( RX_MODE);
    
    // Enable peripheral interrupts
    INTCONbits.PEIE = 1;
}

