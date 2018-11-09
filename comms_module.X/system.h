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

// 10ms = SYS_CLK*0.01 = 15536, so (2^16)-15536 = 0xC350 ~ 10ms
// Interrupt on roll over 0xffff -> 0x0000
#define TMR1H_VAL 0x3C
#define TMR1L_VAL 0xB0

void Initialise( void);

#endif /* SYSTEM_H */
