/*
 * File:   Slave1Main.c
 * Author: vaneg
 *
 * Created on 31 de agosto de 2021, 11:34 PM
 */
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

#define _XTAL_FREQ 4000000
#include <xc.h>
#include "DHT11.h"
#include "I2C.h"


uint8_t temp, hum;

int z;
int contador = 0;
void __interrupt() ISR(){
    if(PIR1bits.SSPIF == 1){ 
        
        SSPCONbits.CKP = 0;
        
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
                       
            SSPCONbits.SSPOV = 0;       
            SSPCONbits.WCOL = 0;       
            SSPCONbits.CKP = 1;         
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
                        
                    
            SSPCONbits.CKP = 1;       
            while(!SSPSTATbits.BF);     
            PORTD = SSPBUF;             
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
           
            z =SSPBUF;
            BF = 0;
            switch(contador){
                case 0:
                    SSPBUF =  temp;
                    break;

                case 1:
                    SSPBUF =  hum;
                    break;
                        
            }
             
            SSPCONbits.CKP = 1;
            while(SSPSTATbits.BF);
            __delay_us(250);
        }
        PIR1bits.SSPIF = 0;
        contador++;
        if (contador == 2) contador = 0;
    }
    
}


void main(void) {
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISB  = 0;
    PORTB = 0;
    PORTA = 0;
    TRISDbits.TRISD0 = 1;
    
    T1CONbits.T1CKPS = 0; // 1:1
    T1CONbits.TMR1CS = 0; 
    TMR1H  = 0;           // reset Timer1
    TMR1L  = 0;
    INTCONbits.GIE =1;
    INTCONbits.PEIE = 1;
    PIE1bits.SSPIE = 1;
    
 
    I2C_Slave_Init(0b00000000);
    while(1){
        dht11_config();
        dht11_read(&hum,&temp);
        PORTB = 1;
        PORTA = temp;
        __delay_ms(100);
    }
    
    return;
}
