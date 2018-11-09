/* 
 * File:   user.h
 * Author: Andrew Baker
 *
 * Created on 20 October 2018, 17:34
 */

#ifndef USER_H
#define	USER_H

#include <stdint.h>

void EdgeIntr( void);
void BitIntr( void);
void TickIntr( void);
void UARTInit( void);
void UARTSend( uint8_t DataValue);
void StartTickTimer( uint8_t ReSync);

extern uint8_t IntrNewBit;
extern uint8_t IntrData;
extern uint8_t RxData;


#ifndef _XTAL_FREQ
 // This definition is required to calibrate __delay_us() and __delay_ms()
 #define _XTAL_FREQ   FOSC   
#endif
#define Baudrate              2400                 //bps
#define OneBitDelay           (1000000/Baudrate)
#define DataBitCount          8                         // no parity, no flow control
#define UART_RX               RA1						// UART RX pin
#define UART_RX_DIR			  TRISAbits.TRISA1			// UART RX pin direction register
#define UART_TX               RA0						// UART TX pin
#define UART_TX_DIR			  TRISAbits.TRISA0			// UART TX pin direction register


#endif	/* USER_H */

