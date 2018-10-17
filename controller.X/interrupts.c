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
        TimeUpdate();
        INTCONbits.T0IF=0;        
    }

    if (PIR1bits.TMR1IF==1)
    {
        BitUpdate();
        PIR1bits.TMR1IF=0;        
    }

}


