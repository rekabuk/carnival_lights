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

    
    // Edge state change interrupt
    if (INTCONbits.INTF)
    {
        INTCONbits.INTF = 0;
#if 1        
        EdgeIntr();
#else
        RC4=1;  // Debug
        RC4=0;

        // Clear for every edge - Used to timeout packet reception
        EdgeDetect = 0;

        // Next TMR0 interrupt will be a data one
        //BitData = 1;

        //Start data timer 30 us   (front porch 20us, data 20us, back porch 20us)
        TMR0 = 106; // 256-150 
        // Clear and enable timer interrupt
        INTCONbits.T0IF = 0;
        INTCONbits.T0IE = 1;    
#endif        
    }
#if 1    
    // Timer 0 - Bit Interrupt
    if (INTCONbits.T0IF==1)
    {
        INTCONbits.T0IF= 0;
        BitIntr();
    }
#endif    
}

