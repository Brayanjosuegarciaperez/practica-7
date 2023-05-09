
#include <Servo.h>
#include <RTClib.h>
#include <Wire.h>
#include <Keypad.h> // 
#include <LiquidCrystal.h>

RTC_DS3231 rtc;

const int rs = 2;
const int en = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;
const byte ROWS = 4; 
const byte COLS = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// definir los símbolos en los botones de los teclados
char hexaKeys[ROWS][COLS] = {  
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte colPins[ROWS] = {11, 10, 9, 8}; 
byte rowPins[COLS] = {A0, A1, A2, A2}; 

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

char clave[4]={'1','2','3','4'};
char datoLeido[4];

const int CERRADURA = 10 ;

Servo servoMotor;
void cambiarPSWD(void);
void setup() {
  // habilitar el numero de filas y columnas del LCD
  
  // Escribe el mensaje en la pantalla lcd
  lcd.setCursor(0, 0);
  lcd.print("PUERTA CERRADA");
  delay(3000);
  

  delay(1000);
  lcd.setCursor(5,1);
   rtc.begin();
   rtc.adjust (DateTime (2023, 05, 7, 12, 30, 00));
  //////////////////////////////
  
  servoMotor.attach(1);
  servoMotor.write(0);
  pinMode(CERRADURA, OUTPUT);
}

byte i=0;
bool band = false;


void loop(){
DateTime now = rtc.now();


inicio:
  if(band==false){
  char tecla = customKeypad.getKey();
      if (tecla){
        lcd.print('*');
        datoLeido[i]=tecla;
        delay(100);
        i++;
      }
      if(i>3){
        band=true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Procesando...");
        lcd.setCursor(0, 1);
        lcd.print("Hora: ");
        lcd.print(now.hour(), DEC);
        lcd.print(":");
        lcd.print(now.minute(), DEC);
       delay(1000);

        delay(3000);
        lcd.clear();
        
      }
    
  }
  else{  
    
    bool ev = (datoLeido[0]==clave[0])&&
              (datoLeido[1]==clave[1])&&
              (datoLeido[2]==clave[2])&&
              (datoLeido[3]==clave[3]);
    if(ev==true){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Contra correcta");  
       lcd.setCursor(0, 1);
        lcd.print("Hora: ");
        lcd.print(now.hour(), DEC);
        lcd.print(":");
        lcd.print(now.minute(), DEC);
      delay(2500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PUERTA ABIERTA");
       lcd.setCursor(0, 1);
        lcd.print("Hora: ");
        lcd.print(now.hour(), DEC);
        lcd.print(":");
        lcd.print(now.minute(), DEC);
       
      delay(3000);
     
       

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("1) abrir");
         lcd.setCursor(0, 1);
        lcd.print("Hora: ");
        lcd.print(now.hour(), DEC);
        lcd.print(":");
        lcd.print(now.minute(), DEC);
    
        
         while(1){
          char teclaSelec = customKeypad.getKey();
            if(teclaSelec == '1'){
              break;
            }
         }
        
        servoMotor.write(90);
        digitalWrite(CERRADURA, 1);
        delay(2000);
        servoMotor.write(0);
        digitalWrite(CERRADURA, 0);
        delay(2000);
        reiniciarSistema();
    }else{
      lcd.clear();
      lcd.setCursor(0, 0);
        lcd.print("Clave Incorrecta"); 
         lcd.setCursor(0, 1);
        lcd.print("Hora: ");
        lcd.print(now.hour(), DEC);
        lcd.print(":");
        lcd.print(now.minute(), DEC);
       
        delay(3000);
        lcd.clear();
        delay(300);
        reiniciarSistema();
    }
      
  }

}

void reiniciarSistema(){
    lcd.clear();
  	lcd.print("PUERTA CERRADA");
    delay(100);
    lcd.setCursor(5,1);
    i=0;
  band = false;
}

void cambiarPSWD(void){
  int conta = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.setCursor(5,1);
  while(1){
    char tecla = customKeypad.getKey();
    if(tecla == '*'){
      conta--;
      if(conta<0){
        conta=0;
      }
      lcd.setCursor(5+conta,1);
      lcd.print(' ');
    }else if((tecla == '#')&&(conta==4)){
      for(int i = 0; i<4; i++){
          clave[i] = datoLeido[i];
      }
      break;
    }else if(tecla && (conta < 4)){
        lcd.setCursor(5+conta,1);
        lcd.print(tecla);
        datoLeido[conta]=tecla;
        delay(100);
        conta++;
      }
  }
}
