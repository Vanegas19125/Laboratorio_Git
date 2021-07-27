/* 
 * File:   
 * Author: electrosome
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LCD_8BITS_H
#define	LCD_8BITS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include<stdint.h>
#define RS RE2
#define EN RE0

#define D0 RD0
#define D1 RD1
#define D2 RD2
#define D3 RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7


#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000

#endif


 void LcdPort(char a);
 void LcdClear(void);
 void LcdSetCursor(char a, char b);
 void LcdInit(void);
 void LcdWriteChar(char a);
 void LcdWriteString( const char *a);
 void LcdShiftRight(void);
 void LcdShiftLeft(void);
 

#endif	/* XC_HEADER_TEMPLATE_H */
