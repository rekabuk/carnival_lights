
#include <xc.h>
#include "system.h"
#include "user.h"
#include "main.h"

const uint8_t BOX_ADDRESS = 0;

//uint8_t BitData;
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

DATA_MC_STATE_T DataState;
uint8_t Addressed;


// Enable for TX or RX
#pragma interrupt_level 1
void BitDataInit( uint8_t ModeTx)
{
     // Store interrupt enable status and disable interrupts
    uint8_t IntEnable = INTCONbits.GIE;
    INTCONbits.GIE = 0;
    
    if (ModeTx == TX_MODE)
    {
        // Disable interrupt on RXD rising edge    
        INTCONbits.INTE = 0; 

        // Set transceiver to RX
        RC1 = 1;
    }
    else
    {
        // Set transceiver to RX
        RC1 = 0;
        
        // Initialise RX state machine
        DataState = DATA_SYNC;
        Addressed = 0;
        BitCount = 0;
        
        // Enable interrupt on RXD rising edge    
        INTCONbits.INTE = 1;    
    }
    
    // Set interrupts back to previous state
    INTCONbits.GIE =  IntEnable;
}



void main(void) {
    
    Initialise();
        
    // Enable interrupt on RDX bit low-to high
    BitDataInit( RX_MODE);    

    // Enable interrupt and away we go
    INTCONbits.GIE = 1;
    
    while(1)
    {
        if (IntrNewBit==1)
        {
            //RC3=1;
            //RC3=0;

            // Clear semaphores
            INTCONbits.T0IE = 0;
            RxData = IntrData;
            IntrNewBit = 0;
            INTCONbits.T0IE = 0;
            
            // New bit so increment count
            BitCount++;
        
            // What shall we do with this new bit?
            if (DataState==DATA_SYNC)
            {   
                // Re-Sync tick count and H/W timer.
    //            TickCount = 0;
                // TODO reset HW timer
                // Debug
                //RC4=1;
                //RC4=0;

                // Sync Word
                if ((RxData&0xF)==0x09)
                {
                    // Debug
                    RC5=1;
                    RC5=0;

                    BitCount = 0;
                    Addressed = 0;
                    DataState = DATA_SYNC;                        
                }
            }
            else if (DataState==DATA_ADDRESS)	
            {
                // 5-bit box address
                if (BitCount>=5)
                {
                    BitCount = 0;
                    
                    if ((RxData&0x1F)==BOX_ADDRESS)
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
                if (BitCount>=5)
                {
                    BitCount = 0;
                    
                    BoxSize = RxData&0x1F;
                    DataState = DATA_LAMPS;
                }  
            }	
            else if (DataState==DATA_LAMPS)	
            {
                // n-bit number of lamps
                // TODO Only works for 8-lamp modules
                if (Addressed==1)
                {
                    Lamps = RxData;
                    //RC5=1;
                    //RC5=0;
                }

                if (BitCount>=BoxSize)
                    BitCount = 0;
                    DataState = DATA_ADDRESS;

            }	
        }
    }
    
}
