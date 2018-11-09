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
#pragma interrupt_level 1
void StartTickTimer( uint8_t ReSync)
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
    // But don't restart if we are transmitting
    if ((++EdgeDetect >= 2) && (!ReSync) && (OneWireMode==RX_MODE))
        BitDataInit( RX_MODE); 

    // Now process the interrupt
    if ((TickCount>=9) || (ReSync))
        TickCount = 0;
    else
        TickCount++;

#ifdef __DEBUG_OFF
    RC4 = (TickCount==8) ? 1 : 0;
    
    static uint8_t Fred;
    Fred=(Fred==0)?1:0;
    RC3 = Fred;
#endif
}


// Called when rising edge detected on RXD
void  EdgeIntr( void)
{
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
    // Shift data and capture new bit
    IntrData <<= 1;
    if (RA2==1)
        IntrData |= 1;

    // Flag we have new data
    IntrNewBit = 1;
}


// Initialise the UART pins/variables
void UARTInit(void)		// Initialise UART pins to proper values
{
	UART_TX = 1;			// TX pin is high in idle state
	
	UART_RX_DIR = 1;		// Input
	UART_TX_DIR = 0;		// Output
}


// Send data to base unit
void UARTSend( uint8_t DataValue)
{
	// Send Start Bit
	UART_TX = 0;
	__delay_us(OneBitDelay);

	for ( uint8_t i = 0; i < DataBitCount; i++ )
	{
		//Set Data pin according to the DataValue
		UART_TX = (DataValue&0x1) ? 1 : 0;
        DataValue >>= 1;
	    __delay_us(OneBitDelay);
	}

	//Send Stop Bit
	UART_TX = 1;
	__delay_us(OneBitDelay);
}
