
#include <xc.h>
#include "system.h"
#include "user.h"
#include "main.h"
#include "interrupts.h"


// System designed for 8 boxes each of 8 channels and 4 boxes of 32 channels
//
// 5-bit Sync + (8 * (5-bit address + 5-bit size + 8-bit data)) + (4 * (5-bit address + 5-bit size + 32-bit data))
// 5 + (8*(5+5+8)) + (4*(5+5+32)) = 317
// 317 bits @ 60us/b = 19ms
//
//
//
//

#define BOX_ADDRESS 3

uint8_t TickCount;
uint8_t BitCount;
uint8_t BoxSize;
uint8_t Lamps;
uint8_t DoneSend;
uint8_t OneWireMode;
uint8_t Addressed;

typedef enum {
    DATA_SYNC = 0,
    DATA_ADDRESS = 1,
    DATA_SIZE = 2,
    DATA_LAMPS = 3
} DATA_MC_STATE_T;

DATA_MC_STATE_T DataState;


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

        // Set transceiver to TX
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

    DoneSend = 0;
    
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
                // Sync Word
                if ((RxData&0xF)==0x09)
                {
#ifdef __DEBUG                    
                    // Debug
                    RC5=1;
                    RC5=0;
#endif
                    // Start next parameter capture
                    BitCount = 0;
                    Addressed = 0;
                    DataState = DATA_ADDRESS;  
                    
                    // Resync the Tick Timer
                    INTCONbits.GIE = 0;
                    StartTickTimer( 1);
                    INTCONbits.GIE = 1;
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
#ifdef __DEBUG                        
                        RC3=1;
                        RC3=0;
#endif
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
                    //
                    // Lamps[BitCount/8] = RxData
                    Lamps = RxData;
#ifdef __DEBUG
                    RC4=1;
                    RC4=0;
#endif
                }

                if (BitCount>=BoxSize)
                {
                    BitCount = 0;
                    DataState = DATA_ADDRESS;
                }
            }	
        }
        
        // In the last 10ms of a 100ms period we need to send data to the other PIC
        if ((TickCount==8) && (DoneSend==0))
        {
            UARTSend( Lamps);
            DoneSend = 1;
        }
        else
        {
            DoneSend = 0;
        }
    }
    
}
