#include "LCD.h"

void LcdPort(char value) {
    //Utilizamos la operaciona and para determinar el valor del bit
    //Luego utilizamos un shift logico para colocarlo en el primer bit
    D0 = value &1;
    D1 =  (value & 2) >> 1;
    D2 = (value & 4) >> 2;
    D3 = (value &8) >> 3;
    D4 = (value & 16)>>4;
    D5 = (value & 32) >> 5 ;
    D6 = (value & 64) >> 6;
    D7 = (value & 128) >> 7;
}

void LcdCmd(char value) {
    RS = 0;     
    LcdPort(value);    
    EN = 1; 
    __delay_ms(4);
    EN = 0; 
}

void LcdClear() {
    LcdCmd(0);
    LcdCmd(1);
}

void LcdSetCursor(char row, char column) {
    switch(row){
        case 1:
            LcdCmd(0x80 + column - 1);
            break;
        case 2:
            LcdCmd(0xC0 + column - 1);
            break;
    }
    
}

void LcdInit() {
    LcdPort(0x00);
    __delay_ms(20);
    LcdCmd(0x30);
    __delay_ms(5);
    LcdCmd(0x30);
    __delay_ms(11);
    LcdCmd(0x30);
    
    LcdCmd(0x38); 
    LcdCmd(0x0C);
    LcdCmd(0x6);

}

void LcdWriteChar(char value) {
    RS = 1; 
    LcdPort(value); 
    EN = 1;
    __delay_us(40);
    EN = 0;
}

void LcdWriteString(const char *value) {
    
    for (int i = 0; value[i] != '\0'; i++)
        LcdWriteChar(value[i]);
}

void LcdShiftRight() {
    LcdCmd(0x01);
    LcdCmd(0x0C);
}

void LcdShiftLeft() {
    LcdCmd(0x01);
    LcdCmd(0x08);
}
