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
    CMCON = 0x07;               // Disable comparator 
    TRISA = 0x3F;               // All inputs RA2=RXD
    OPTION_REGbits.INTEDG = 1;  // Rising edge interrupt enable on RA2
   
    // Port C bits    
    TRISC = 0;              // All outputs RC1=TEX, RC2= TXD RC3=Test, RC4=Test
    
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
    INTCONbits.PEIE = 1;
    
    
    // TMR0 is for bit timing, use Fosc/4 and no prescaler
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 1;

    // Enable change of state detection on RA2
    //IOCA = 0x04;
    
#if 0        
    INTCONbits.INTE = 1;    // Enable External interrupt
#endif    

}

