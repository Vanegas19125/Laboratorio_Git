/* 
 * File:   Libreria.c
 * Author: Jose Vanegas
 *
 * Created on 13 de julio de 2021, 12:49 PM
 */
#include <stdint.h>
#include <pic.h>
#include <pic16f887.h>
#include<xc.h>

#ifndef DIGITAL2_TOOLBOX_H
#define	DIGITAL2_TOOLBOX_H
/*Librerias implementadas en todos los laboratorios*/

//|----------------------------------ADC---------------------------------------|

uint16_t readAnalog();
uint8_t EightBitAnalog();


//|----------------------------------LCD---------------------------------------|

//USAR HD44780 en PORTD y PORTE
#ifndef RS
#define RS RE0
#endif

#ifndef EN
#define EN RE1
#endif

#ifndef RW
#define RW RE2
#endif

void    Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(char *a);
void Lcd_Cmd(uint8_t a);
void cursor_Pos(uint8_t a);

#endif	/* DIGITAL2_TOOLBOX_H */

//|---------------------------------UART---------------------------------------|

void UART_Init(void);
void UART_Write(unsigned char* word);
void UART_Write_Char(uint8_t character);
uint8_t UARTDataReady();
char UARTReadChar();
//uint8_t UARTReadString(char *buf, uint8_t max_length);

//|----------------------------------SPI---------------------------------------|
  
#ifndef __SPI_H
#define	__SPI_H

typedef enum 
{
    SPI_MASTER_OSC_DIV4  = 0b00100000,
    SPI_MASTER_OSC_DIV16 = 0b00100001,
    SPI_MASTER_OSC_DIV64 = 0b00100010,
    SPI_MASTER_TMR2      = 0b00100011,
    SPI_SLAVE_SS_EN      = 0b00100100,
    SPI_SLAVE_SS_DIS     = 0b00100101
}Spi_Type;

typedef enum
{
    SPI_DATA_SAMPLE_MIDDLE   = 0b00000000,
    SPI_DATA_SAMPLE_END      = 0b10000000
}Spi_Data_Sample;

typedef enum
{
    SPI_CLOCK_IDLE_HIGH  = 0b00010000,
    SPI_CLOCK_IDLE_LOW   = 0b00000000        
}Spi_Clock_Idle;

typedef enum
{
    SPI_IDLE_2_ACTIVE    = 0b00000000,
    SPI_ACTIVE_2_IDLE    = 0b01000000
}Spi_Transmit_Edge;


void spiInit(Spi_Type, Spi_Data_Sample, Spi_Clock_Idle, Spi_Transmit_Edge);
void spiWrite(char);
unsigned spiDataReady();
char spiRead();

#endif	/* SPI_H */

//|-------------------------------MSSP I2C-------------------------------------|

 /*Project         : I2C Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Link: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Modificada por: Pablo Mazariegos con la ayuda del auxiliar Gustavo Ordo??ez 
 * Basado en Link: http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __I2C_H
#define	__I2C_H

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 4000000
#endif

//*****************************************************************************
// Funci??n para inicializar I2C Maestro
//*****************************************************************************
void I2C_Master_Init(const unsigned long c);
//*****************************************************************************
// Funci??n de espera: mientras se est?? iniciada una comunicaci??n,
// est?? habilitado una recepci??n, est?? habilitado una parada
// est?? habilitado un reinicio de la comunicaci??n, est?? iniciada
// una comunicaci??n o se este transmitiendo, el IC2 PIC se esperar??
// antes de realizar alg??n trabajo
//*****************************************************************************
void I2C_Master_Wait(void);
//*****************************************************************************
// Funci??n de inicio de la comunicaci??n I2C PIC
//*****************************************************************************
void I2C_Master_Start(void);
//*****************************************************************************
// Funci??n de reinicio de la comunicaci??n I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart(void);
//*****************************************************************************
// Funci??n de parada de la comunicaci??n I2C PIC
//*****************************************************************************
void I2C_Master_Stop(void);
//*****************************************************************************
//Funci??n de transmisi??n de datos del maestro al esclavo
//esta funci??n devolver?? un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
void I2C_Master_Write(unsigned d);
//*****************************************************************************
//Funci??n de recepci??n de datos enviados por el esclavo al maestro
//esta funci??n es para leer los datos que est??n en el esclavo
//*****************************************************************************
unsigned short I2C_Master_Read(unsigned short a);
//*****************************************************************************
// Funci??n para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(uint8_t address);
//*****************************************************************************
#endif	/* __I2C_H */

//|------------------------------Miscelaneous----------------------------------|

void divide(uint16_t value, uint8_t *mil, uint8_t *cent, uint8_t *dec, uint8_t *unit);