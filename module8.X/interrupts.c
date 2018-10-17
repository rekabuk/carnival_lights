/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>             /* XC8 General Include File */
#include <stdint.h>         /* For uint8_t definition */
#include "user.h"           /* User funct/params, such as InitApp */

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/
void __interrupt() isr(void)
{    
    if (INTCONbits.T0IF==1)
    {
        SequenceUpdate();
        INTCONbits.T0IF=0;        
    }
}



