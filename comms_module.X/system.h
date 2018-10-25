/* 
 * File:   system.h
 * Author: Andrew Baker
 *
 * Created on 20 October 2018, 17:24
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#define FOSC 20000000
#define SYS_CLK  FOSC / 4

// 20000000/4/50000  = 50000 = 0xC350 ~ 1ms
#define TMR1H_VAL 0xC3
#define TMR1L_VAL 0x50

void Initialise( void);

#endif /* SYSTEM_H */
