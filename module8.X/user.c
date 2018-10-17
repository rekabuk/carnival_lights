/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

void InitApp(void)
{
    // Disable comparator
    CMCONbits.CM = 7;
    
    // Lamp control outputs
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;

    /* TODO Initialize User Ports/Peripherals/Project here */

    /* Setup analog functionality and port direction */

    /* Initialize peripherals */

    /* Enable interrupts */
}

#pragma interrupt_level 1
void SetLamps( uint8_t Lamps)
{
    if (Lamps & 0x01)
        RA1 = 1;
    else
        RA1 = 0;
        
    if (Lamps & 0x02)
        RC5 = 1;
    else
        RC5 = 0;
        
    if (Lamps & 0x04)
        RC4 = 1;
    else
        RC4 = 0;
        
    if (Lamps & 0x08)
        RC3 = 1;
    else
        RC3 = 0;
        
    if (Lamps & 0x10)
        RC2 = 1;
    else
        RC2 = 0;
        
    if (Lamps & 0x20)
        RC1 = 1;
    else
        RC1 = 0;
        
    if (Lamps & 0x40)
        RC0 = 1;
    else
        RC0 = 0;
        
    if (Lamps & 0x80)
        RA2 = 1;
    else
        RA2 = 0;
        
}

void SequenceUpdate( void)
{
    static uint8_t TimeCount = 0;
    static uint8_t SequenceIndex = 0;
    const uint8_t Sequence[4] = {0x77, 0xBB, 0xDD, 0xEE};
    
    // This will run every second
    if (++TimeCount==4)
    {
        TimeCount = 0;
        
        if (++SequenceIndex == sizeof(Sequence))        
            SequenceIndex = 0;
  
        SetLamps(Sequence[SequenceIndex]);
    }
            
}