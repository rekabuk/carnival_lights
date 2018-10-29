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
void SendModule( void);
void StartTickTimer( void);

extern uint8_t IntrNewBit;
extern uint8_t IntrData;
extern uint8_t RxData;


#endif	/* USER_H */

