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
    // Initialise TMR1
    StartTickTimer();

    BitDataInit();
    
    // Enable peripheral interrupts
    INTCONbits.PEIE = 1;
}