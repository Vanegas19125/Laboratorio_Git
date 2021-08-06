/*
 * File:   newmain.c
 * Author: Angel Orellana
 *
 * Created on 1 de febrero de 2021, 05:37 PM
 */

#include "ADC.h"
#include <stdint.h>

void initADC(){
    // dependiendo del valodr de XTAL se colocara un valor u otro en la 
    // frecuencia del oscildor
    
    #if _XTAL_FREQ <= 1000000
    ADCON0bits.ADCS = 0;
    #elif _XTAL_FREQ <= 4000000
    ADCON0bits.ADCS = 1;
    #else
    ADCON0bits.ADCS = 2;
    #endif
    
    //PIE1bits.ADIE = 1; // interrupcion activada
    ADCON1bits.ADFM = 0; // justificado a la izquierda
    ADCON1bits.VCFG1 = 0;
    ADCON1bits.VCFG0 = 0; // referencias en la alimentacion
    ADCON0bits.ADON = 1; // se enciende

    return;
}
#if ADC_BITS == 8
uint8_t readADC(uint8_t pin){
    ADCON0bits.CHS = pin;
    ADCON0bits.GO = 1; // se inicia
    while(ADCON0bits.GO); // se queda aqui hasta que se termine la conversion
    return ADRESH;
}
#else
int readADC(uint8_t pin){
    ADCON0bits.CHS = pin;
    ADCON0bits.GO = 1; // se inicia
    while(ADCON0bits.GO); // se queda aqui hasta que se termine la conversion
    return (ADRESH<<2) | (ADRESL>>6);
}
#endif