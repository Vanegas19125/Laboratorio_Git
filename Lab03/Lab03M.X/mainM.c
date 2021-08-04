/*
 * File:   mainM.c
 * Author: Jose Vanegas
 *
 * Created on 30 de julio de 2021, 09:44 AM
 */
#// CONFIG1
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
#include<stdint.h>
#include <stdlib.h>
#include "spi.h"
#include "USART.h"
#define _XTAL_FREQ 4000000

#include <xc.h>

//Valor de los comandos

#define GIVE_ADC 'A'
#define GIVE_ADC2 'B'

//Aqui obtenemos la lectura del ADC
uint8_t valueADC = 0;
char* cadenaADC;
uint8_t valueADC2= 0;

//-------------------------------------------------------------
char caracteres[4];
char auxiliar;
uint8_t contador = 0;
//-----------prototipos de funciones--------------------------- 
char* adcToString(uint16_t);
char* int2String(uint8_t);


void __interrupt() isr(){
    if(PIR1bits.RCIF){
        auxiliar = RCREG;
        caracteres[contador] = auxiliar;
        if(auxiliar =='\n') contador = 0;
        else contador++;
        
    }
}


int main(){
  //como no se utilizara el ADC en el master se desactivan
  ANSEL = 0;
  ANSELH = 0;
  // PUERTOS PARA CONTADOR 8 BITS
  TRISA = 0;
  
  INTCONbits.GIE = 1;
  INTCONbits.PEIE = 1;
  PIE1bits.RCIE = 1;
  // INICIALIZACION DE LOS MODULOS A USAR
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE,
                SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    
  UARTInit(9600,1);
  
  
  while(1){
      
      if(auxiliar == '\n') PORTA = atoi(caracteres);
      
    spiWrite(GIVE_ADC);
    valueADC = spiRead();
 

    cadenaADC = adcToString(valueADC);
    UARTSendString("POT 1: ",20);
    UARTSendString(cadenaADC,20);
    
    
    spiWrite(GIVE_ADC2);
    valueADC2 = spiRead();
    
    cadenaADC = adcToString(valueADC2);
    UARTSendString("   POT 2: ",20);
    UARTSendString(cadenaADC,20);
    UARTSendString("\n",20);
    __delay_ms(250);
  }
  
   
  return 0;
}



char* adcToString(uint16_t value){
    char salida[5]; //cadena que sera retornada
    uint16_t digito;
    
    value = value*1.96; // se cambia de 0-255  a 0-500
    digito = value/100 ; // unidad de voltaje
    
    salida[0] = digito + 48;
    salida[1] = '.';
    value -= digito*100;
    
    digito = value/10; // decimas
    salida[2] = digito +48;
    digito = value % 10;
    salida[3] = digito +48;
    salida[4] = '\0';
    return salida;
}

char* int2String(uint8_t value){
    char cadena[4]; //cadena en donde se almacenan los ASCCI de los digitos
    uint8_t digito;
    // se obtiene el valor de las decenas y s e convierte a ASCII
    digito = value/100;
    cadena[0] = digito +48;
    value-= digito*100;
    // se hace lo mismo con las decenas y centenas
    cadena[1] = value/10 + 48;
    cadena[2] = value%10 + 48;
    cadena[3] = '\0';
    return cadena;
}
