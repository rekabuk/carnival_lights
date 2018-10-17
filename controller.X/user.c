/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>             /* XC8 General Include File */
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"
#include "system.h"

// Transmit state machine
#define TX_IDLE         0
#define TX_START        1
#define TX_FRONT_GUARD  2
#define TX_DATA         3
#define TX_REAR_GUARD   4
#define TX_FINISH       5
uint8_t TxMode = TX_IDLE;

#define DATA_START      0
#define DATA_UNIT_ADDR  1
#define DATA_UNIT_SIZE  2
#define DATA_UNIT_DATA  3
#define DATA_FINISH     4
uint8_t DataMode = DATA_START;

uint8_t SerialModeTx = 0;
uint8_t TxBit = 0;

// Define the units
typedef struct  {
    uint8_t Address;
    uint8_t Lamps;
    uint8_t LampData;  
} UNIT_T;

#define NUM_UNITS 4
UNIT_T Units[NUM_UNITS];



void TimeUpdate(void)
{
    static uint8_t Count = 0;
    
    if (Count++ == 9)
        Count = 0;
    
    // Transmit every ~100 ms
    if (Count == 0)
    {
        StartTx();
    }
}

// This deals with TX and RX
void BitUpdate(void)
{
    uint8_t DataByte;
    uint8_t DataMask;
    
    if (TxMode == TX_IDLE)
    {
        // Nothing to do
        return;
    }
    else if (TxMode == TX_START)          // Enable TX
    {
        TX_PIN = 0;
        TXE_PIN = 1;
        
        DataMode = DATA_START;
        DataMask = 0;
        
        TxMode = TX_FRONT_GUARD;
    }
    else if (TxMode == TX_FRONT_GUARD)    // Line high
    {
        // If mask is zero then we can start the next byte
        if (DataMask == 0)
        {
            if (DataMode == DATA_START)
            {
                // Set up preamble in bottom four bits
                DataByte = 0x9;
                DataMask = 0x08;
                
                // Do address next
                DataMode = DATA_UNIT_ADDR;
            }
            else if (DataMode == DATA_UNIT_ADDR)
            {
                // Send 5-Bit Addr
                DataByte = 0x3;
                DataMask = 0x10;
                
                // Do size next
                DataMode = DATA_UNIT_SIZE;
            }        
            else if (DataMode == DATA_UNIT_SIZE)
            {
                // Send 5-bit Size
                DataByte = 0x07;      // Number of lamps -1
                DataMask = 0x10;
                
                // Do Lamp Data next
                DataMode = DATA_UNIT_DATA;
            }
            else if (DataMode == DATA_UNIT_DATA)
            {
                // Send Data bits
                DataByte = 0x55;
                DataMask = 0x80;
                
                DataMode = DATA_FINISH;
            }
        }
        
        if (DataMask != 0)
        {        
            TX_PIN = 1;
            TxMode = TX_DATA;
        }
        else
        {
            TX_PIN = 0;
            TxMode = TX_FINISH;
        }
        
    }
    else if (TxMode == TX_DATA)          // Output Data
    {        
        // Output the bit
        if (DataByte & DataMask)
            TX_PIN = 1;
        else
            TX_PIN = 0;

        // Set mask ready for next bit
        DataMask >>= 1;

        TxMode = TX_REAR_GUARD;
    }
    else if (TxMode == TX_REAR_GUARD)    // Line high
    {
        TX_PIN = 0;        
        TxMode = TX_FRONT_GUARD;              // Yes        
    }
    else if (TxMode == TX_FINISH)       // Line high
    {
        TXE_PIN = 0;
        TxMode = TX_IDLE;
    }
}

void SetUnitConfig(uint8_t Unit, uint8_t Address, uint8_t Lamps )
{
    Units[Unit].Address = Address;
    Units[Unit].Lamps = Lamps;
}

void SetUnitLamps(uint8_t Unit, uint8_t LampData )
{
    Units[Unit].LampData = LampData;    
}

// Start the transmission of a sequence
void StartTx( void)
{
    // Set the Timer for TX timeout = 20us
    
    // Set TX mode to true
    SerialModeTx = 1; 
    TxMode = TX_START;
    
    // Set timer to Tx mode (20us) and enable interrupt
    
}