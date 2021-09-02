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
void main(void) {
    ANSEL = 0;
    ANSELH = 0;
    
    TRISD = 0;
    TRISE = 0;
    TRISB = 8;
    TRISA = 0;
    
    //PORTD = 255;
    PORTA = 0;
    LcdInit();
    UARTInit(9600,1);
    I2C_Master_Init(100000);
    
    LcdSetCursor(1,1);
    LcdWriteString("Hall: Temp: Hum:");
    while(1){
        I2C_Master_Start();
        I2C_Master_Write(0b00000011);
        hall = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(10);
        LcdSetCursor(2,1);
        PORTA = hall;
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
        
        
        itoa(numero,hum,10);
 
        LcdWriteChar(' ');
        LcdWriteString(numero);
        LcdWriteChar('%');
        
    }
    
    
    return;
}
