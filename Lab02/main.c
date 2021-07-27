/*
 * File:   main.c
 * Author: Jose Vanegas
 *
 * Created on 21 de Julio de 2021, 10:40 AM
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

#include <xc.h>
#include<stdint.h>
#include "LCD.h"
#include "ADC.h"
#include "USART.h"
#define _XTAL_FREQ 4000000
char* analogToString(uint8_t value);
char* intToString(uint8_t value);


int main(){
  TRISD = 0;
  TRISE = 0;
  ANSEL = 3;
  ANSELH = 0;
  LcdInit();
  initADC();
  UARTInit(9600,1);
  volatile char *val;
  volatile char *val2;
  char *contadorString;
  uint8_t *adc1;
  uint8_t *adc2;
  
  uint8_t contador = 0;
  while(1){
      
    //--Leemos los valores del ADC y los escribimos en la LCD--
    adc1 = readADC(0);
    
    LcdSetCursor(1,1); // lo dejamos un momento
    adc2 = readADC(1); //Leemos el 
    LcdWriteString("S1:   S2:   S3:");
    LcdSetCursor(2,1);
    // --------En esta parte se imprimen los valores -----------------
    val = analogToString(*adc1);
    LcdWriteString(val);
    LcdWriteString("V ");
    // -------Enviamos el valor por UART--------
    UARTSendString(val,6);
    UARTSendString("V ",3);
    val2 = analogToString(*adc2);
    LcdWriteString(val2);
    LcdWriteString("V ");
    //-- Lo enviamos de nuevo, ya que val 1 y val 2 hacen referencia a val
    UARTSendString(val,6);
    UARTSendString("V \n",3);
    
    
    // ----------En esta parte verificamos si hay algo en el UART--------
    
    if (UARTDataReady()){
        char entrada  = UARTReadChar();
        if( entrada == '+'){
            contador++;
        }
        else if (entrada =='-'){
            contador--;
        }
    }
        contadorString = intToString(contador);
    LcdWriteString(contadorString);
   
  }
  return 0;
}
// Aqui imprimiremos los valores con 2 decimales de presicion.
char* analogToString(uint8_t value){
    float valor =  value *0.019;
    char string[5]; 
    uint8_t entero =  valor;
    
    
    string[0] = entero + 48;// Imprimimos el entero
    string[1]= '.'; //Aqui el punto decimal
    valor =( valor - entero); // Aqui eliminamos la parte entera y se corre el 
    // punto decimal
    valor*=10;
    entero =  valor  ; // Tomamos solo la parte entera
    string[2] = entero + 48;
    
    valor -= entero;
    valor*=10;
    
    entero =  valor  ; //  tomamos la parte entera
    string[3] = (entero + 48);
    string[4] ='\0';
    return string;
}


char* intToString(uint8_t value){
    char valor[4];
    
    uint8_t entero = value/100; // El numero de centenas
    valor[0] = entero+48;
    
    value -= 100*entero; // Quitamos las decenas
    
    valor[1] = value/10 + 48 ; // Valor de las decenas
    valor[2] = value%10  + 48;// El resto
    valor[3] = '\0'; //Caracter Nulo
    
    return valor;
          
    
    
}


void __interrupt() isr(){
    isrADC();
}