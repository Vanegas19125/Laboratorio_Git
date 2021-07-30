/*
 * File:   main.c
 * Author: Jose Vanegas
 *
 * Created on 30 de julio de 2021, 01:36 AM
 */
// CONFIG1
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#include <stdint.h>
#include "spi.h"
#include "ADC.h"
#include "USART.h"  

uint8_t adcValue, adcValue2;
uint8_t valor;
char* cadena;

void main(void) {
    ANSEL = 3; //ANS0 como entrada analogica
    ANSELH= 0;
    INTCONbits.GIE =1;
    INTCONbits.PEIE =1; //perifericos activados
    PIE1bits.SSPIE = 1; //interrupcion en MSSP activada
    
    
    spiInit(SPI_SLAVE_SS_DIS,SPI_DATA_SAMPLE_MIDDLE,
            SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    initADC();
    
    while(1){
        adcValue = readADC(0);
        adcValue2 = readADC(1);
        
    }
    
    return;
}

void __interrupt() isr(){
    if(PIR1bits.SSPIF ==1){ // si se recibio un dato
        
        valor = spiRead();
        if (valor=='A') spiWrite(adcValue);
        else if (valor=='B') spiWrite(adcValue2);
       
        PIR1bits.SSPIF = 0;
    }
}