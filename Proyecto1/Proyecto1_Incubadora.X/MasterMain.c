/*
 * File:   MasterMain.c
 * Author: vaneg
 *
 * Created on 31 de agosto de 2021, 11:33 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

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
#define _XTAL_FREQ 4000000
#include <xc.h>
#include <stdlib.h>
#include "LCD.h"
#include "USART.h"
#include "I2C.h"

char hall=0;

char temp, hum;
char numero[10];


uint8_t menu =0; // indica si se tiene que mostrar la configuracion o el sensores
uint8_t opcion = 0; // 0 temperatura 1 humedad.
uint8_t incrementar, decrementar, ok;
char umbralTemp=32, umbralHum=20; 
char portbAnterior =31,portbActual=31;
uint8_t contador = 0;
uint8_t direccion = 0;

void step(char steps,  char dir);


void __interrupt() ISR(){
    if(PIR1bits.TMR2IF){
        
        contador++;
        PIR1bits.TMR2IF = 0;
        
    }
}

void main(void) {
    ANSEL = 0;
    ANSELH = 0;
    
    TRISD = 0;
    TRISE = 0;
    TRISB = 31;
    OPTION_REGbits.nRBPU= 0;
    TRISA = 0;
    
    //PORTD = 255;
    PORTA = 0;
    LcdInit();
    UARTInit(9600,1);
    I2C_Master_Init(100000);
    // configuracion del timer 2
    PR2 =255;
    T2CONbits.TOUTPS = 0b1111;
    T2CONbits.T2CKPS = 3;
    T2CONbits.TMR2ON = 1;
    
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.TMR2IE = 1; // interrupciones activdas
    
    while(1){
        portbAnterior = portbActual;
        portbActual = PORTB;
        // verificacion de que se presionaron los botones
        if((portbAnterior&1) == 0 && (portbActual&1) == 1 ) menu = ~menu;
        if((portbAnterior&2) == 0 && (portbActual&2) == 2 ) decrementar = 1;
        if((portbAnterior&4) == 0 && (portbActual&4) == 4 ) incrementar = 1;
        if((portbAnterior&8) == 0 && (portbActual&8) == 8 ) ok = 1;
        
        if(contador == 45){
            
            contador = 0;
            step(128,direccion);
            
            direccion = ~direccion&1;
        }
        
        
        if(!menu){
            LcdSetCursor(1,1);
            LcdWriteString("Hall: Temp: Hum:");
            I2C_Master_Start();
            I2C_Master_Write(0b00000011);
            hall = I2C_Master_Read(0);
            I2C_Master_Stop();
            __delay_ms(10);
            LcdSetCursor(2,1);
    
            switch(hall){
                case 0:
                    LcdWriteString("open ");
                    break;
                case 1:
                    LcdWriteString("close");
                    break;
            }
            // obteniendo temperatura y humedad
            I2C_Master_Start();
            I2C_Master_Write(0b00000001);
           temp = I2C_Master_Read(0);
            I2C_Master_Stop();
            __delay_ms(10);

            I2C_Master_Start();
            I2C_Master_Write(0b00000001);
            hum = I2C_Master_Read(0);
            I2C_Master_Stop();
            __delay_ms(10);



            itoa(numero,temp,10);
            LcdWriteChar(' ');
            LcdWriteChar(' ');
            LcdWriteString(numero);
            LcdWriteChar(0xDF);
            LcdWriteChar('C');

            if(hum > umbralHum){
                itoa(numero,hum,10);

                LcdWriteChar(' ');
                LcdWriteString(numero);
                LcdWriteChar('%');
                LcdWriteChar(' ');
            }else{
                LcdWriteString(" N.A");
            }
            
            
            
            if(temp > umbralTemp) PORTA |= 48;
            else if(temp <= (umbralTemp-5))PORTA &= ~48;
        }else{
            LcdSetCursor(1,1);
            LcdWriteString(" Selec. opcion: ");
            LcdSetCursor(2,1);
            if(incrementar || decrementar){
                opcion = ~opcion;
                incrementar = decrementar = 0;
            }
            if(opcion)LcdWriteString(" Temp  >humedad");
            else LcdWriteString(">Temp   humedad");
            
            if(ok){
                ok = 0;
                
                while(ok == 0){
                    portbAnterior = portbActual;
                    portbActual = PORTB;


                    if((portbAnterior&2) == 0 && (portbActual&2) == 2 ) decrementar = 1;
                    if((portbAnterior&4) == 0 && (portbActual&4) == 4 ) incrementar = 1;
                    if((portbAnterior&8) == 0 && (portbActual&8) == 8 ) ok = 1;
                    
                    
                    if(opcion){
                        
                        if (incrementar){
                            incrementar = 0;
                            umbralHum++;
                        }
                        if(decrementar && umbralHum>=0){
                            decrementar = 0;
                            umbralHum--;
                        }
                        LcdSetCursor(1,1);
                        LcdWriteString("  Hum. minima:  ");
                        LcdSetCursor(2,1);
                        LcdWriteString("  ");
                        itoa(numero,umbralHum,10);
                        LcdWriteString(numero);
                        LcdWriteString("%                      ");
                    }else{
                        LcdSetCursor(1,1);
                        LcdWriteString("  Temp. Maxima: ");
                        if (incrementar){
                            incrementar = 0;
                            umbralTemp++;
                        }
                        if(decrementar && umbralTemp>=0){
                            decrementar = 0;
                            umbralTemp--;
                        }
                        
                        LcdSetCursor(2,1);
                        LcdWriteString("  ");
                        itoa(numero,umbralTemp,10);
                        LcdWriteString(numero);
                        LcdWriteChar(0xDF);
                        LcdWriteString("C                   ");
                    }
                    
                }
                ok = 0;
            }
            
        }
                
    }
    
    
    return;
}


void step(char steps,  char dir){
    
    switch(dir){
        case 0:
            for (int i = 0; i<steps; i++){
                PORTA &= ~14;
                PORTA |= 1;
                __delay_ms(10);
                PORTA &= ~13;
                PORTA |= 2;
                __delay_ms(10);
                PORTA &= ~11;
                PORTA |= 4;
                __delay_ms(10);
                PORTA &= ~5;
                PORTA |= 8;
                __delay_ms(10);
            }
            break;
        case 1:
            for (int i = 0; i<steps; i++){
                PORTA &= ~5;
                PORTA |= 8;
                __delay_ms(10);
                
                PORTA &= ~11;
                PORTA |= 4;
                __delay_ms(10);
                
                
                PORTA &= ~13;
                PORTA |= 2;
                __delay_ms(10);
                
                
                PORTA &= ~14;
                PORTA |= 1;
                __delay_ms(10);
                
            }
            break;
            
    }
    
    
}
