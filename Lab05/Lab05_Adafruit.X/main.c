/*
 * File:   main.c
 * Author: Jose Vanegas
 *
 * Created on 13 de agosto de 2021, 01:34 PM
 */
// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ  4000000
#include<xc.h>
#include<stdint.h>
#include<pic.h>
#include<string.h>
#include"Libreria.h"

//Variables

uint8_t contador;
uint8_t contador2;
uint8_t mc;
uint8_t cc;
uint8_t dc;
uint8_t uc;


void setup(void);
void __interrupt() isr(void);


void    main(void){
    setup();
    while(1){
        
        PORTD   =   contador;
        
        if(contador2  != contador){
            divide(contador, &mc, &cc, &dc, &uc);
            contador2    =   contador;
            if(cc!=0)
            UART_Write_Char(cc+48);
            if(dc!=0)
            UART_Write_Char(dc+48);
            
            UART_Write_Char(uc+48);
            UART_Write("\r");
        }
        
        
        if(RCIF){
            PORTA   =   RCREG;
        }
    }
}

//Configuracioon de los puertos e Interrupciones
void setup(void){
   
    ANSEL   =   0;
    ANSELH  =   0;
    TRISA   =   0;
    TRISB   =   3;
    TRISC   =   128;
    TRISD   =   0;
    TRISE   =   0;
    
    
    OPTION_REGbits.nRBPU    =   0;
    WPUB    =   0b00000011;
    IOCB    =   0b00000011;

    
    GIE     =   1;
    RBIE    =   1;
    RBIF    =   0;
    
    UART_Init();
    
    PORTA   =   0;
    PORTB   =   0;
    PORTC   =   0;
    PORTD   =   0;
    PORTE   =   0;
    
    contador =   0;
    contador2    =   255;
}

//Interrupciones

void __interrupt() isr(void){
    if(RBIF){
        if(RB0==0 && RB1==1){
            contador++;
        }
        if(RB0==1 && RB1==0){
            contador--;
        }
        RBIF    =   0;
        
    }

}