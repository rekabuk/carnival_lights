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

const uint8_t BOX_ADDRESS = 0;

uint8_t BitData;
uint8_t BitCount;
uint8_t Sync;
uint8_t Address;
uint8_t BoxSize;
uint8_t Lamps;
uint8_t EdgeDetect;

typedef enum {
    DATA_SYNC = 0,
    DATA_ADDRESS = 1,
    DATA_SIZE = 2,
    DATA_LAMPS = 3
} DATA_MC_STATE_T;

uint8_t Data;
DATA_MC_STATE_T DataState;
uint8_t Addressed;

// Enable tick timer on TMR1
void StartTickTimer( void)
{
    // Stop TMR1
    T1CONbits.TMR1ON = 0;
    // Reset timer
    TMR1H = TMR1H_VAL;
    TMR1L = TMR1L_VAL;
    // Clear interrupt 
    PIR1bits.TMR1IF = 0;
    // Enable timer
    T1CONbits.TMR1ON = 1;
    
    // If there have been no edges for two ticks
    // then restart the RX state machine
    if (++EdgeDetect >= 2)
        BitDataInit( RX_MODE);        
}

// Enable for TX or RX
#pragma interrupt_level 1
void BitDataInit( uint8_t ModeTx)
{
    uint8_t IntEnable;
    uint8_t Dummy;
    
    // Store interrupt enable status and disable interrupts
    IntEnable = INTCONbits.GIE;
    INTCONbits.GIE = 0;
    
    if (ModeTx == TX_MODE)
    {
        // Disable change of state interrupt
        INTCONbits.RAIE = 0;

        // Set transceiver to RX
        RC1 = 1;
    }
    else
    {
        // Set transceiver to RX
        RC1 = 0;
        
        // Initialise RX state machine
        BitData = 1;
        DataState = DATA_SYNC;
        Data = 0;
        Addressed = 0;
        
        // Read RXD to set current latched state ready for change
        Dummy = PORTA;
        
        // Enable interrupt on RXD bit state change    
        INTCONbits.RAIE = 1;
    }
    
    // Set interrupts back to previous state
    INTCONbits.GIE =  IntEnable;
}


// Called when rising edge detected on RXD
void  EdgeIntr( void)
{
    // Disable edge interrupts
    INTCONbits.RAIE = 0;
    
    RC4=1;  // Debug
    RC4=0;
    
    // Clear for every edge - Used to timeout packet reception
    EdgeDetect = 0;
    
    // Next TMR0 interrupt will be a data one
    BitData = 1;
    
    //Start data timer 30 us   (front porch 20us, data 20us, back porch 20us)
    TMR0 = 106; // 256-150 
    // Clear and enable timer interrupt
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;    
}

// Called by timer 30us after RXD bit changes state
// then again 20us later in the middle of the guard band
// so that the change of state interrupt may be reset
void BitIntr( void)
{
    RC3=1;
    RC3=0;

    // Is this an interrupt to sample the data?
    if (BitData==1)
    {
        // Capture the data
        // Reading RA2 will clear the interrupt on change bit
        Data = (Data<<1) | RA2;

        // Set the timer to interrupt in the guard band so we can reset the 
        // port change interrupt
        // Start data timer 20 us   (front porch 20us, data 20us, back porch 20us)
        TMR0 = 156; // 256-100 
        // Clear and enable timer interrupt
        INTCONbits.T0IF = 0;
        INTCONbits.T0IE = 1;    
        
        // Next interrupt will be in the guard bit
        BitData = 0;

        if (DataState==DATA_SYNC)
        {   
            // Re-Sync tick count and H/W timer.
            TickCount = 0;
            // TODO reset HW timer
            
            // Debug
            //RC4=1;
            //RC4=0;

            // Sync Word
            // 0x9 = Default message   <SYNC>[<<ADDR><SIZE><DATA>>....]
            if (++BitCount==4)
            {
                 if ((Data&0xF)==0x9)
                 {
                    Data = 0;
                    DataState = DATA_ADDRESS;
                 }
            }  
        }
        else if (DataState==DATA_ADDRESS)	
        {
            // 5-bit box address
            if (++BitCount==5)
            {
                if (Data==BOX_ADDRESS)
                {
                    Addressed = 1;
                    //RC3=1;
                    //RC3=0;
                }
                else
                {
                    Addressed = 0;
                }

                DataState = DATA_SIZE;            
            }  
        }	
        else if (DataState==DATA_SIZE)	
        {
            // 5-bit number of lamps
            if (++BitCount==5)
            {
                if (Data < 31)
                    BoxSize = Data;

                DataState = DATA_LAMPS;
            }  
        }	
        else if (DataState==DATA_LAMPS)	
        {
            // n-bit number of lamps
            // TODO Only works for 8-lamp modules
            if (Addressed==1)
            {
                Lamps = Data;
                RC5=1;
                RC5=0;
            }

            if (++BitCount==BoxSize)
                DataState = DATA_ADDRESS;

        }	
    }
    // Otherwise it's to reset change-of-state
    else
    {
        // Next timer interrupt will be a data bit
        BitData = 1;
        // Read port to reset change of state
        uint8_t Dummy = PORTA;
        // Enable change of state interrupt
        INTCONbits.RAIE = 0;
    }
}

// Send data to base unit
// Called @ 90ms into period
void SendModule( void)
{
    
}
