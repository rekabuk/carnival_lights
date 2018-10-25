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

const uint8_t BOX_ADDRESS = 0;

uint8_t BitCount;
uint8_t Sync;
uint8_t Address;
uint8_t BoxSize;
uint8_t Lamps;

typedef enum {
    DATA_SYNC = 0,
    DATA_ADDRESS = 1,
    DATA_SIZE = 2,
    DATA_LAMPS = 3
} DATA_MC_STATE_T;

uint8_t Data;
DATA_MC_STATE_T DataState;
uint8_t Addressed;

void BitDataInit( void)
{
    DataState = DATA_SYNC;
    Data = 0;
	Addressed = 0;
}


void StartTickTimer( void)
{
    uint8_t IntEnable;
    
    // Store interrupt enable status
    IntEnable = INTCONbits.GIE;obal interrupts
    INTCONbits.GIE = 0;
    
    // Stop TMR1
    T1CONbits.TMR1ON = 0;
    // Clear interrupt 
    INTCONbits.T0IF = 0;
    //Restart timer
    TMR1H = TMR1H_VAL;
    TMR1L = TMR1L_VAL;
    T1CONbits.TMR1ON = 1;
    
    // Set interrupts back to previous state
    INTCONbits.GIE =  IntEnable;
}

void  EdgeIntr( void)
{
    //INTCONbits
    TMR1H = 2;
    TMR1L = 0;
    // Start data timer 30 us   (front porch 20us, data 20us, back porch 20us)
    TCON0bits.TMR0ON = 1;
}

// danny danny  daddy pollty,

void BitIntr( void)
{
    // Grab the data
    Data = (Data<<1) | RA0;

    // Stop bit data timer
    T1CONbits.TMR1ON = 0;
    
    if (DataState==DATA_SYNC)
    {   
        // Re-Sync tick count and H/W timer.
        TickCount = 0;
                
        // Sync Word
        // 0x9 = Default message   <SYNC>[<<ADDR><SIZE><DATA>>....]
        if (++BitCount==4)
        {
             if (Data==0x9)
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
                Addressed = 1;
            else
                Addressed = 0;
            
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
            Lamps = Data;
        
        if (++BitCount==BoxSize)
            DataState = DATA_ADDRESS;
        
    }	

}

// Send data to base unit
// Called @ 90ms into period
void SendModule( void)
{
    
}
