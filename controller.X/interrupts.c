/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/


void __interrupt() isr(void)
{
    if (INTCONbits.T0IF==1)
    {
        TMR0 = 61;                         
        INTCONbits.T0IF=0;  
        TimeUpdate();        
    }

    if (PIR1bits.TMR2IF==1)
    {
        PR2 = 100;
        PIR1bits.TMR2IF=0;        
        BitUpdate();
    }

}


