#include "Libreria.h"
#include <stdint.h>
#include <pic.h>
#define _XTAL_FREQ  4000000

/*Para este laboratorio se busco unificar todas las librerias utilizadas
 hasta el momento, la libreria del ADC, LCD, I2C, USART, etc*/

//-------------------------------ADC-------------------------------------------
uint16_t readAnalog(){
    uint16_t result = 0;
    uint16_t a;
    uint16_t b;
    a   =   ADRESL>>6;
    b   =   ADRESH<<2;
    result   = b|a;
    return result;
}

uint8_t EightBitAnalog(){       
    uint8_t a = ADRESH;
    return a;
}
//------------------------------------------------------------------------------
//--------------------------------LCD-------------------------------------------
void Lcd_Cmd(uint8_t a) {
    RS = 0; // 
    PORTD   =   a;
    EN = 1; // 
    __delay_ms(4);
    EN = 0; //
}

void Lcd_Clear(void) {
    Lcd_Cmd(0);
    Lcd_Cmd(1);
}


void Lcd_Init(void) {
    PORTD   =   0;
    __delay_ms(20);
    Lcd_Cmd(0x030);
    __delay_ms(5);
    Lcd_Cmd(0x030);
    __delay_us(160);
    Lcd_Cmd(0x030);

    Lcd_Cmd(0b00111000);        
    Lcd_Cmd(0x10);              
    Lcd_Cmd(0x01);              
    Lcd_Cmd(0b00000110);        
    Lcd_Cmd(0x10);              
    Lcd_Cmd(0b00001100);        
    __delay_ms(100);
}

void Lcd_Write_Char(char a) {
    PORTD   =   a;
    RS = 1;     
    EN = 1;
    __delay_us(100);
    EN = 0;
    __delay_ms(10);
}

void Lcd_Write_String(char *a) {
    int i;
    for (i = 0; a[i] != '\0'; i++){
        Lcd_Write_Char(a[i]);
    }
}

//-----------------------------------------------------------------------------
//--------------------------------UART-----------------------------------------

void UART_Init(void){
    
    TXSTAbits.TXEN  =   1;     
    TXSTAbits.SYNC  =   0;      
    RCSTAbits.SPEN  =   1;     
    TXSTAbits.BRGH  =   1;      
    BRG16   =   0;
    SPBRGH  =   0;              
    SPBRG   =   25;
    
       
    TRISCbits.TRISC7    =   1;  
    RCSTAbits.CREN  =   1;      
}
    
void UART_Write(unsigned char* word){   
    while (*word != 0){                 
        TXREG = (*word);               
        while(!TXSTAbits.TRMT);         
        word++;                        
    }
}

void UART_Write_Char(uint8_t character){
    TXREG   =   character;
    while   (!TXSTAbits.TRMT);
}
uint8_t UARTDataReady() {
    return PIR1bits.RCIF;
}

/**
 * Read one character
 * @return  byte read
 */
char UARTReadChar() {
    while (!UARTDataReady());   // Wait for data to be available
    return RCREG;
}
//uint8_t UARTReadString(char *buf, uint8_t max_length) {
//    uint8_t i = 0;
//    char tmp = 1;
//    for (i=0 ; i<max_length-1 ; i++) {
//        tmp = UARTReadChar();
//        // Stop reading if end of string is read
//        if (tmp == '\0' || tmp == '\n' || tmp == '\r') {
//            break;
//        }
//        buf[i] = tmp;
//    }
//    
//    buf[i+1] = '\0';
//    
//    return i;
//-----------------------------------------------------------------------------
//-----------------------------SPI by Electrosome.-----------------------------
void spiInit(Spi_Type sType, Spi_Data_Sample sDataSample, Spi_Clock_Idle sClockIdle, Spi_Transmit_Edge sTransmitEdge)
{
    TRISC5 = 0;
    if(sType & 0b00000100) //If Slave Mode
    {
        SSPSTAT = sTransmitEdge;
        TRISC3 = 1;
    }
    else              //If Master Mode
    {
        SSPSTAT = sDataSample | sTransmitEdge;
        TRISC3 = 0;
    }
    
    SSPCON = sType | sClockIdle;
}

static void spiReceiveWait()
{
    while ( !SSPSTATbits.BF ); // Wait for Data Receive complete
}

void spiWrite(char dat)  //Write data to SPI bus
{
    SSPBUF = dat;
}

unsigned spiDataReady() //Check whether the data is ready to read
{
    if(SSPSTATbits.BF)
        return 1;
    else
        return 0;
}

char spiRead() //REad the received data
{
    spiReceiveWait();        // wait until the all bits receive
    return(SSPBUF); // read the received data from the buffer
}

//------------------------------------------------------------------------------
//-------------------------------I2C--------------------------------------------
 /*
 * File            : I2C.c
 * Author          : Ligo George
 * Company         : electroSome
 * Project         : I2C Library for MPLAB XC8
 * Microcontroller : PIC 16F877A
 * Created on April 15, 2017, 5:59 PM
 * Link: https://electrosome.com/i2c-pic-microcontroller-mplab-xc8/
 * Modificada por: Pablo Mazariegos con la ayuda del auxiliar Gustavo Ordoñez 
 * Basado en Link: http://microcontroladores-mrelberni.com/i2c-pic-comunicacion-serial/
 */
//I2C Maestro
void I2C_Master_Init(const unsigned long c)
{
    SSPCON = 0b00101000;
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1;
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;
    TRISCbits.TRISC4 = 1;
}
//*****************************************************************************
// Función de espera: mientras se esté iniciada una comunicación,
// esté habilitado una recepción, esté habilitado una parada
// esté habilitado un reinicio de la comunicación, esté iniciada
// una comunicación o se este transmitiendo, el IC2 PIC se esperará
// antes de realizar algún trabajo
//*****************************************************************************
void I2C_Master_Wait()
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}
//*****************************************************************************
// Función de inicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Start()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.SEN = 1;    //inicia la comunicación i2c
}
//*****************************************************************************
// Función de reinicio de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_RepeatedStart()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RSEN = 1;   //reinicia la comunicación i2c
}
//*****************************************************************************
// Función de parada de la comunicación I2C PIC
//*****************************************************************************
void I2C_Master_Stop()
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.PEN = 1;    //detener la comunicación i2c
}
//*****************************************************************************
//Función de transmisión de datos del maestro al esclavo
//esta función devolverá un 0 si el esclavo a recibido
//el dato
//*****************************************************************************
void I2C_Master_Write(unsigned d)
{
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPBUF = d;             
}
//*****************************************************************************
//Función de recepción de datos enviados por el esclavo al maestro
//esta función es para leer los datos que están en el esclavo
//*****************************************************************************
unsigned short I2C_Master_Read(unsigned short a)
{
    unsigned short temp;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    temp = SSPBUF;
    I2C_Master_Wait();      //espera que se cumplan las condiciones adecuadas
    if(a == 1){
        SSPCON2bits.ACKDT = 0;
    }else{
        SSPCON2bits.ACKDT = 1;
    }
    SSPCON2bits.ACKEN = 1;          // Iniciar sequencia de Acknowledge
    return temp;                    // Regresar valor del dato leído
}
//*****************************************************************************
// Función para inicializar I2C Esclavo
//*****************************************************************************
void I2C_Slave_Init(uint8_t address)
{ 
    SSPADD = address;
    SSPCON = 0x36;      // 0b00110110
    SSPSTAT = 0x80;     // 0b10000000
    SSPCON2 = 0x01;     // 0b00000001
    TRISC3 = 1;
    TRISC4 = 1;
    GIE = 1;
    PEIE = 1;
    SSPIF = 0;
    SSPIE = 1;
}
//*****************************************************************************

//|-----------------------------Miscelaneous-----------------------------------|

void divide(uint16_t value, uint8_t *mil, uint8_t *cent, uint8_t *dec, uint8_t *unit){
    *mil=value/1000;
    *cent=(value-1000*(int)*mil)/100;
    *dec=(value-1000*(int)*mil-100*(int)*cent)/10;
    *unit=value-1000*(int)*mil-100*(int)*cent-10*(int)*dec;
    
}

//------------------------------------------------------------------------------