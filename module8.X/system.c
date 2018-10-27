/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/


#include <xc.h>         /* XC8 General Include File */
// #include <plib.h>  
 #include <stdlib.h>  
#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"

/* Refer to the device datasheet for information about available
oscillator configurations. */
void ConfigureOscillator(void)
{
    OPTION_REGbits.T0CS = 0;           // Timer increments on instruction clock
    OPTION_REGbits.PSA = 0;            // Use Pre-scaler with instruction clock
    OPTION_REGbits.PS = 7;             // Pre-scaler = 32
    TMR0 = 48;                         // Give 53.5ms interrupt
    INTCONbits.T0IE = 1;               // Enable interrupt on TMR0 overflow
}
