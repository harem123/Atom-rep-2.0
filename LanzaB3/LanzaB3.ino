#include <LiquidCrystal.h>

// Inicialización de libreria con los pines lcd
LiquidCrystal lcd(22, 23, 24, 25, 26, 27);

int IN3 = 33; //Define pin de salida para control de la entrada 3 puente h
int IN4 = 34; //Define pin de salida para control de la entrada 4 puente h

//**Variables para transmisión serial**//
const int En_WrRd_RS485 =  2;
const int Led_1 =  13; 
 
char VarChar = ' ';
String BufferIn = "";  
String NumIn = "";      
boolean StringCompleta = false; 

//**Banderas y variables**//
int altura = -1; //Guarda valor de altura actual
int altura_p = 0; //Guarda valor de altura previa
int altura_pp = 0; //Guarda valor de altura previa previa
int seg = 0; //Variable para determinar el cambio de altura
int seg_p = 0; //Variable para determinar el cambio de altura anterior
int time_on = 0; //Almacena el tiempo de encendido
int velocidad = -1; //Guarda valor de velocidad actual

unsigned long currentmillis;
unsigned long previousmillis;

boolean flag_enable1 = false;
boolean flag_enable2 = false;
boolean flag_enable3 = false;
boolean flag_startA = false; //Habilita función f_altura
boolean flag_startV = false; //Habilita función f_velocidad
boolean IN_3=true; //Permite definir el estado de la salida IN3
boolean IN_4=false; //Permite definir el estado de la salida IN4
boolean flag_control = false;

void setup() {
  // put your setup code here, to run once:
  //**Configuraciones para pwm a 4khz**//
  int myEraser = 7;             // this is 111 in binary and is used as an eraser
  TCCR2B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0
  int myPrescaler = 2;         // this could be a number in [1 , 6]. In this case, 3 corresponds in binary to 011.   
  TCCR2B |= myPrescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
  //**fin configuaciones pwm**//
  
  Serial.begin(9600);// inicio serial a 9600 bauds
  BufferIn.reserve(5); // reservo al menos 5 bytes para el buffer

  pinMode(En_WrRd_RS485, OUTPUT);
  digitalWrite(En_WrRd_RS485, LOW); 
  Serial.println("IN");

  pinMode(IN4, OUTPUT);    // Entrada4 conectada al pin 4 
  pinMode(IN3, OUTPUT);    // Entrada3 conectada al pin 3
  
  lcd.begin(16, 2); //Configura lcd de 16 columnas x 2 filas
  //**Menú de inicio del programa**//
  lcd.setCursor(0, 0); //Ubica el cursor
  lcd.print("Altura:"); //ND: no definido
  lcd.setCursor(0, 1);
  lcd.print("Velocidad:");
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(flag_enable1 == true)
    currentmillis = millis()-previousmillis;

  if(currentmillis >= time_on && flag_enable1 == true) //Apaga el motor de altura al cumplir el tiempo time_on
  {
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, LOW);
    flag_enable1 = false;
  }
  
  if(flag_startA == true) //Entrada a la función de altura
  {
    f_altura();
    flag_startA = false;
  }

  if(flag_startV == true) //Entrada a la función de velocidad
  {
    f_velocidad();
    flag_startV = false;
  }
}

//**Función f_velocidad genera pwm requerido**//
void f_velocidad()
{
  switch (velocidad) {
    case 1:
      //do something when var equals 0
      analogWrite(10,80);
      break;
    case 2:
      //do something when var equals 1
      analogWrite(10,127);
      break;
    case 3:
      //do something when var equals 2
      analogWrite(10,254);
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }
}

//**Función f_altura establece el tiempo time_on para el control de altura, se trabaja con altura 0, 1 y 2 para min, medio, maximo**//
void f_altura()
{
  
  seg = altura-altura_p;
 
  if(seg!=0) //Configura solo si hay cambio de altura
  {
    if(seg < 0)
      seg = -1*seg;
    currentmillis = millis()-previousmillis; //Determina el tiempo transcurrido desde la ultima configuración de altura no repetida
    previousmillis = millis();
    if(flag_control == false) //Configuración para la primera entrada cuando se reinicia el programa
    {
    currentmillis = 14000;     
    flag_control = !flag_control; 
    }
    if(currentmillis > 14000) //Para controlar que el valor maximo de tiempo será 14000 ms
      currentmillis = 14000;
    
    if(altura == 0) //Configura y determina time_on para ir a la altura 0
    {
      IN_3 = true;
      IN_4 = false;
      digitalWrite (IN3, IN_3);
      digitalWrite (IN4, IN_4); 
      if(currentmillis < seg_p) //Evalua si el equipo no llegó a la altura anterior
      {
        if(altura_p == 1)
        {
          if(altura_pp == 2)
            time_on = 14000 - currentmillis;
          else
            time_on = currentmillis;
        }
        if(altura_p == 2)
        {
          if(altura_pp ==1)
            time_on = 7000 + currentmillis;
          else
          time_on = currentmillis;
        }
      }
      else //Si el equipo llegó a la altura anterior entonces..
        time_on = 7000*seg;
    }
    
    if(altura == 2) //Configura y determina time_on para ir a la altura 2
    {
      IN_3 = false;
      IN_4 = true;
      digitalWrite (IN3, IN_3);
      digitalWrite (IN4, IN_4);  
      if(currentmillis < seg_p) //Evalua si el equipo no llegó a la altura anterior
      {
        if(altura_p == 1)
        {
          if(altura_pp == 0)
            time_on = 14000 - currentmillis;
          else
            time_on = currentmillis;
        }
        if(altura_p == 0)
        {
          if(altura_pp ==1)
            time_on = 7000 + currentmillis;
          else 
          time_on = currentmillis;
        }
      }
      else //Si el equipo llegó a la altura anterior entonces..
        time_on = 7000*seg;
    }
    
    if(altura == 1) //Configura y determina time_on para ir a la altura 1
    {
      if(altura_pp==1) //Evalua si altura previa previa fue 1
      {
        if(currentmillis < 7000)
          currentmillis = currentmillis + 7000;
        else
          currentmillis = 14000;
      }

      time_on=7000-currentmillis;
      /*if(time_on > 0)
      {
        wd_timer_id=timer.setTimeout(time_on, apagar);
        timer.enable(wd_timer_id);
      }*/
      if(time_on < 0)
      {
        time_on=-1*time_on;
        IN_3=!IN_3;
        IN_4=!IN_4;
        digitalWrite (IN3, IN_3);
        digitalWrite (IN4, IN_4); 
      }
    }
    seg_p = seg*7000; 
    flag_enable1 = true; 
    altura_pp=altura_p;
    altura_p = altura;      
  }
  
}

void serialEvent() {

  while (Serial.available()) 
  {
    VarChar = (char)Serial.read();
    BufferIn += VarChar;
    if ( (isDigit(VarChar)) )
    {
      NumIn += VarChar;
    }
    if (VarChar == 'Z') { StringCompleta = true; }   
  }

  //*Instrucciones a realizar si se recibió datos por el puerto serial*//
  if (StringCompleta) 
  {      
      delay(50);
      digitalWrite(En_WrRd_RS485, LOW); 
      //Serial.println(BufferIn);
      //Serial.println(NumIn);
           
      if ((BufferIn.indexOf('M')) >= 0)
      {
        if ((BufferIn.indexOf('A')) >= 0)
        {
          int serialValue = (NumIn.toInt());
          if(serialValue > -1 && serialValue < 3)
          {
            altura = serialValue;
            lcd.setCursor(8, 0);
            lcd.print(altura);
            flag_enable2 = true;
          }
          //flag_enable1 = false;
          //flag_startA = true;
        }
        if ((BufferIn.indexOf('V')) >= 0)
        {
          int serialValue = (NumIn.toInt());
          if(serialValue > -1 && serialValue < 3)
          {
            velocidad = serialValue;
            lcd.setCursor(11, 1);
            lcd.print(velocidad);
            flag_enable3 = true;
          }
          //flag_enable1 = false;
          //flag_startA = true;
        }
        if(flag_enable2 == true && flag_enable3 == true)
        {
          if ((BufferIn.indexOf('I')) >= 0)
          {
            
            flag_startA = true;
            flag_startV = true;
          }
        }
      }
    //Reinicia variables
    StringCompleta = false;
    BufferIn = "";
    NumIn = "";     
  }
}
