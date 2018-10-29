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

uint8_t TickCount;

void __interrupt() ISR( void)
{
#if 0
    // Timer 1 - Tick interrupt
    if (PIR1bits.TMR1IF==1)
    {
        StartTickTimer();
               
        // Now process the interrupt
        if (TickCount==9)
        {
            TickCount = 0;
            SendModule();
        }
        else
        {
            TickCount++;
        }
    }
#endif    
    // Edge state change interrupt
    if (INTCONbits.INTF)
    {
        INTCONbits.INTF = 0;
        EdgeIntr();
    }
#if 0    
    // Timer 0 - Bit Interrupt
    if (INTCONbits.T0IF==1)
    {
        BitIntr();
    }
#endif    
}

