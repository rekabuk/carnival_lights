/* 
 * File:   user.c
 * Author: Andrew Baker
 *
 * Created on 20 October 2018, 17:24
 */

#include <xc.h>
#include <stdint.h>

#include "interrupts.h"
#include "system.h"
#include "user.h"
#include "main.h"


uint8_t IntrNewBit;
uint8_t IntrData;
uint8_t RxData;
uint8_t EdgeDetect;


// Enable tick timer on TMR1
void StartTickTimer( void)
{
    // Stop TMR1
    T1CONbits.TMR1ON = 0;
    // Clear interrupt 
    PIR1bits.TMR1IF = 0;
    // Reset timer
    TMR1H = TMR1H_VAL;
    TMR1L = TMR1L_VAL;
    // Enable timer
    T1CONbits.TMR1ON = 1;
    
    // If there have been no edges for two ticks
    // then restart the RX state machine
    if (++EdgeDetect >= 2)
        BitDataInit( RX_MODE);        
}


// Called when rising edge detected on RXD
void  EdgeIntr( void)
{
    //RC4=1;  // Debug
    //RC4=0;
    
    // Clear for every edge - Used to timeout packet reception
    EdgeDetect = 0;

    //Start data timer 30 us   (front porch 20us, data 20us, back porch 20us)
    TMR0 = 175; // 256-150 
    // Clear and enable timer interrupt
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;       
}

// Called by timer 30us after RXD bit changes state
// then again 20us later in the middle of the guard band
// so that the change of state interrupt may be reset
void BitIntr( void)
{
    // Debug
    RC3=1;
    RC3=0;

    // Shift data and capture new bit
    IntrData <<= 1;
    if (RA2==1)
        IntrData |= 1;

    // Flag we have new data
    IntrNewBit = 1;
}

// Send data to base unit
// Called @ 90ms into period
void SendModule( void)
{
    
}
