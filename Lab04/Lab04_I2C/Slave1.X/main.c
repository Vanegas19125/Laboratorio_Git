//Slave 1

// Archivo: main.c
// Dispositivo: PIC16F887
// Autor: Jose Vanegas
// Compilador: XC-8 (v2.32)
//
// Programa: I2C
// Hardware: 3 PIC16f887, 2 pots, 4LEDs, LCD, Sensor I2C
//
// Creado 03 de ago, 2021

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
#include<pic.h>
#include<stdint.h>
#include<string.h>
#include"Libreria.h"


//|-------------------------------VARIABLES------------------------------------|

uint8_t analog0;
uint8_t analog0temp;
uint8_t z;



void setup(void);
void __interrupt() isr(void);



void    main(void){
    setup();
    uint16_t    v1;
    uint8_t     m1;
    uint8_t     c1;
    uint8_t     d1;
    uint8_t     u1;

    uint16_t    v0;
    uint8_t     m0;
    uint8_t     c0;
    uint8_t     d0;
    uint8_t     u0;
    while(1){
        GO  =   1;
        __delay_us(100);

        
        if(analog0!=analog0temp){
            analog0temp =   analog0;
            v0    =   (float)5000*(float)analog0/(float)255;
            }
        
        PORTB   =   analog0;
        
        
    }
}
//Funciones

void setup(void){
    
    ANSEL   =   0b00000011;
    ANSELH  =   0;
    TRISA   =   0b00000011;
    TRISB   =   0;
    TRISC   =   0;
    TRISD   =   0;
    TRISE   =   0;
    
    //Configuracion ADC
    ADCON0bits.ADCS    =   0b01;   
    ADCON0bits.CHS     =   0b0000; 
    ADON    =   1;                  
    ADIF    =   0;

    //Configuracion I2C
    I2C_Slave_Init(0b00000000);
    
    //Interrupciones
    GIE     =   1;
    PEIE    =   1;
    ADIE    =   1;
    
    
 
    PORTA   =   0;
    PORTB   =   0;
    PORTC   =   0;
    PORTD   =   0;
    PORTE   =   0;
    
    
}

       
//Interrupciones

void __interrupt() isr(void){
    if(ADIF){
        analog0    =   EightBitAnalog();
        ADIF    =   0;
    }

    if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 
            SSPCONbits.SSPOV = 0;       
            SSPCONbits.WCOL = 0;        
            SSPCONbits.CKP = 1;         
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
           
            z = SSPBUF;                
            PIR1bits.SSPIF = 0;        
            SSPCONbits.CKP = 1;       
            while(!SSPSTATbits.BF);     
            PORTD = SSPBUF;             
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = analog0;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}

