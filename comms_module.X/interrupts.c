/* 
 * File:   interrupts.c
 * Author: Andrew Baker
 *
 * Created on 20 October 2018, 17:24
 */

#include <xc.h>
#include <stdint.h>

#include "user.h"
#include "system.h"


void __interrupt() ISR( void)
{
    // Timer 1 - Tick interrupt
    if (PIR1bits.TMR1IF==1)
        StartTickTimer( 0);
                   
    // Edge state change interrupt
    if (INTCONbits.INTF)
    {
        INTCONbits.INTF = 0;     
        EdgeIntr();
    }
    
    // Timer 0 - Bit Interrupt
    if (INTCONbits.T0IF==1)
    {
        INTCONbits.T0IF= 0;
        BitIntr();
    }
   
}

