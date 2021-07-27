#ifndef ADC_H 
#define	ADC_H 

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
void initADC(void);
uint8_t* readADC(uint8_t pin );
void isrADC(void);

#endif	/* XC_HEADER_TEMPLATE_H */