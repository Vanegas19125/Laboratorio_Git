/*
 * File:   newmain.c
 * Author: Jose Vanegas
 *
 * Created on: 22 de julio de 2021
 */

#include "ADC.h"
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000 
#endif


#ifndef ADC_CHANELS
#define ADC_CHANELS 12 
#endif

uint8_t ADC_VALUE[ADC_CHANELS]; 

void initADC(){
    
    
    #if _XTAL_FREQ <= 1000000
    ADCON0bits.ADCS = 0;
    #elif _XTAL_FREQ <= 4000000
    ADCON0bits.ADCS = 1;
    #else
    ADCON0bits.ADCS = 2;
    #endif
    
    INTCONbits.GIE =1 ; 
    INTCONbits.PEIE =1 ; 
    
    PIE1bits.ADIE = 1; 
    
    ADCON1bits.ADFM = 0; 
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0; 
    ADCON0bits.ADON = 1; 
    return;
}

uint8_t* readADC(uint8_t pin){
    if (ADCON0bits.GO ==0 ){
         ADCON0bits.CHS = pin;
        __delay_us(3);
        ADCON0bits.GO = 1; 
    }
    return &ADC_VALUE[pin]; 
}

void isrADC(){
    if(PIR1bits.ADIF){
        ADC_VALUE[ADCON0bits.CHS] = ADRESH;
        PIR1bits.ADIF = 0;
    }
    return;
}