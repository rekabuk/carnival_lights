/* 
 * File:   user.h
 * Author: Andrew Baker
 *
 * Created on 20 October 2018, 17:34
 */

#ifndef USER_H
#define	USER_H

#include <stdint.h>

#define RX_MODE 0
#define TX_MODE 1
void BitDataInit( uint8_t ModeTx);
void EdgeIntr( void);
void BitIntr( void);
void TickIntr( void);
void SendModule( void);
void StartTickTimer( void);


#endif	/* USER_H */

