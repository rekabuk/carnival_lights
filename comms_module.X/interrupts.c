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
    if (INTCONbits.T0IF==1)
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
    
    if (INTCONbits.RAIF==1)
    {
        EdgeIntr();
    }
    
    if (INTCONbits.GIE==1)
    {
        BitIntr();
    }
    
}

