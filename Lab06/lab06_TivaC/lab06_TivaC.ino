/*Autor: Jose Vanegas
 * Fecha: 22/09/2021
 * 
 */
  int J0AR = 0;
  int J1AR = 0;
  int PUSHJ1 = 31;
  int PUSHJ2 = 17;
  int J0 = 0;
  int J1 = 0;
  int P = 0;
  int GANADOR;
void setup() {
  
  pinMode(PUSHJ1, INPUT_PULLUP);
  pinMode(PUSHJ2, INPUT_PULLUP);
  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(36, OUTPUT);
  pinMode(37, OUTPUT);
  pinMode(18, OUTPUT);

  pinMode(30, OUTPUT);  //LED ROJO
  pinMode(39, OUTPUT);  //LED VERDE
  pinMode(40, OUTPUT);  //LED AZUL
}

void loop() {
  

  if(P == 0){
    digitalWrite(30, HIGH);
    delay(1000);
    digitalWrite(39, HIGH);
    delay(1000);
    digitalWrite(30, LOW);
    P++;
  }

  //--------------------------------------------Jugador 1 ------------------------------------------------------
  while(P == 1){
     if(digitalRead(PUSHJ1) == 0){
      if(J0AR ==  0){
        J0 ++;
      }
      J0AR  = 1;
    }
    if(digitalRead(PUSHJ1) ==  1){
      J0AR  = 0;
    }
    switch(J0){
      case 1:
        digitalWrite(3, HIGH);
        break;
      case 2:
        digitalWrite(3, LOW);
        digitalWrite(4, HIGH);
        break;
      case 3:
        digitalWrite(4, LOW);
        digitalWrite(5, HIGH);
        break;
      case 4:
        digitalWrite(5, LOW);
        digitalWrite(6, HIGH);
        break;
      case 5:
        digitalWrite(6, LOW);
        digitalWrite(7, HIGH);
        break;
      case 6:
        digitalWrite(7, LOW);
        digitalWrite(8, HIGH);
        break;
      case 7:
        digitalWrite(8, LOW);
        digitalWrite(9, HIGH);
        break;
      case 8:
        digitalWrite(9, LOW);
        digitalWrite(10, HIGH);
        GANADOR  = 0;              //JUGADOR 1 GANA
        digitalWrite(30, HIGH);   
        digitalWrite(39, LOW);
        digitalWrite(40, LOW);
        P++;
        break;
      default:
        digitalWrite( 3, LOW);
        digitalWrite( 4, LOW);
        digitalWrite( 5, LOW);
        digitalWrite( 6, LOW);
        digitalWrite( 7, LOW);
        digitalWrite( 8, LOW);
        digitalWrite( 9, LOW);
        digitalWrite( 10, LOW);
        J0  = 0;
    }
  
        //-------------------------------------------------JUGADOR 2----------------------------------------------------------------
    if(digitalRead(PUSHJ2) == 0){
      if(J1AR ==  0){
        J1 ++;
      }
      J1AR  = 1;
    }
    if(digitalRead(PUSHJ2) ==  1){
      J1AR  = 0;
    }
    switch(J1){
      case 1:
        digitalWrite(11, HIGH);
        break;
      case 2:
        digitalWrite(11, LOW);
        digitalWrite(12, HIGH);
        break;
      case 3:
        digitalWrite(12, LOW);
        digitalWrite(13, HIGH);
        break;
      case 4:
        digitalWrite(13, LOW);
        digitalWrite(14, HIGH);
        break;
      case 5:
        digitalWrite(14, LOW);
        digitalWrite(15, HIGH);
        break;
      case 6:
        digitalWrite(15, LOW);
        digitalWrite(36, HIGH);
        break;
      case 7:
        digitalWrite(36, LOW);    
        digitalWrite(37, HIGH);  
        break;
      case 8:
        digitalWrite(37, LOW);
        digitalWrite(18, HIGH);
        GANADOR  = 1;              //JUGADOR 2 GANA
        digitalWrite(30, LOW);    
        digitalWrite(39, LOW);
        digitalWrite(40, HIGH);
        P++;
        break;
      default:
        digitalWrite( 11, LOW);
        digitalWrite( 12, LOW);
        digitalWrite( 13, LOW);
        digitalWrite( 14, LOW);
        digitalWrite( 15, LOW);
        digitalWrite( 36, LOW);
        digitalWrite( 37, LOW);
        digitalWrite( 18, LOW);
        J1  = 0;
    }
  }
 
}
