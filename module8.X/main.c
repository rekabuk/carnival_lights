/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/


#include <xc.h>            /* XC8 General Include File */

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialise I/O and Peripherals for application */
    InitApp();

    SetLamps( 0x00);
    
    // Enable interrupts and off we go
    INTCONbits. GIE = 1;                

    while(1)
    {
        /* TODO <INSERT USER APPLICATION CODE HERE> */
    }

}

