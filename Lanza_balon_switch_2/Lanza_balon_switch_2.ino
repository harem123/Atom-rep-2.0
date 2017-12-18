#include <LiquidCrystal.h>
#include <SimpleTimer.h>
#include <Switch.h> //libreria para uso de pulsadores

// Inicialización de libreria con los pines lcd
LiquidCrystal lcd(22, 23, 24, 25, 26, 27);

SimpleTimer timer;

int button_1=30; //define el pin a usar para boton de nivel
int button_2=31; //define el pin a usar para boton de altura
int button_3=32; //define el pin a usar para boton de ejecución
int IN3 = 33; //Define pin de salida para control de la entrada 3 puente h
int IN4 = 34; //Define pin de salida para control de la entrada 4 puente h
int nivel=-1; //Variable que controla velocidad de motores, se inicializa en -1 para colocar lcd en estado de inicio ND
int altura=-1; //Variable que controla la altura, se inicializa en -1 para colocar lcd en estado de inicio ND
int B_nivel, B_altura, B_aceptar; //Reciben el valor del boton presionado (1 o 0)
int seg; //Variable para determinar el cambio de altura
int altura_p = 0; //Guarda el valor de la altura anterior para determinar el cambio de altura
int wd_timer_id; //Controla la ejecución del timer de la función simpleTimer (watch dog) 

boolean control = false; //Realiza un código de inicio la primera vez que se presiona ejecutar
long intervalo; //Guarda el tiempo transcurrido entre las veces que se presiona el boton ejecutar para evaluar la posición de altura
unsigned long t_previo; //Guarda el el valor millis() cada vez que se presiona el botón ejecutar
int time_on; //Guarda el tiempo en el cual se apagará el motor de altura en selección medio
boolean IN_3=true; //Permite definir el estado de la salida IN3
boolean IN_4=false; //Permite definir el estado de la salida IN4
int altura_pp; //Guarda el valor de altura previo previo

//*Configura los pulsadores mediantes resistencias de pull down*//
Switch button1 = Switch(button_1, INPUT, HIGH);
Switch button2 = Switch(button_2, INPUT, HIGH);
Switch button3 = Switch(button_3, INPUT, HIGH);

void setup() {
  //**Configuraciones para pwm a 4khz**//
  int myEraser = 7;             // this is 111 in binary and is used as an eraser
  TCCR2B &= ~myEraser;   // this operation (AND plus NOT),  set the three bits in TCCR2B to 0
  int myPrescaler = 2;         // this could be a number in [1 , 6]. In this case, 3 corresponds in binary to 011.   
  TCCR2B |= myPrescaler;  //this operation (OR), replaces the last three bits in TCCR2B with our new value 011
  //**fin configuaciones pwm**//
  
  Serial.begin(9600);
  pinMode(IN4, OUTPUT);    // Entrada4 conectada al pin 4 
  pinMode(IN3, OUTPUT);    // Entrada3 conectada al pin 3
  
  lcd.begin(16, 2); //Configura lcd de 16 columnas x 2 filas
  //**Menú de inicio del programa**//
  lcd.setCursor(0, 0); //Ubica el cursor
  lcd.print("Velocidad: ND"); //ND: no definido
  lcd.setCursor(0, 1);
  lcd.print("Altura: ND");

  //**Ubica el motor de altura en la posición 0**//
  digitalWrite (IN3, IN_3); // inicial in_3 = True
  digitalWrite (IN4, IN_4); // inicial in_3 = False
  delay(500); 
}

void loop() {
  timer.run(); //Para funcionamiento de la libreria SimpleTimer
  
  //**lectura de pulsadores**// 
  button1.poll(); 
  button2.poll(); 
  button3.poll(); 
  B_nivel=button1.pushed();
  B_altura=button2.pushed();
  B_aceptar=button3.pushed();
  //**fin lectura de pulsadores**// 

  config_velocidad();
  config_altura();
  inicio_altura();
  inicio_velocidad();
  
  
  //Reinicia el valor de los botones
  B_nivel=0;
  B_altura=0;
  B_aceptar=0;
}

void apagar()
// apaga brazo 
{
    digitalWrite (IN3, LOW);
    digitalWrite (IN4, LOW); 
}

//La función velocidad coloca la variable nivel en 0, 1, o 2 según la elección del usuario 
// e imprime el valor de velocidad seleccionado
void config_velocidad()
{
  if(B_nivel==1)
  {
    nivel++;
    if(nivel==3)
       nivel=0;
    lcd.setCursor(11, 0);
    switch (nivel) {
    case 0:
      //do something when var equals 0
      lcd.print("Bajo ");
      break;
    case 1:
      //do something when var equals 1
      lcd.print("Medio");
      break;
    case 2:
      //do something when var equals 2
      lcd.print("Alto ");
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
    }
  }
}

//La función altura coloca la variable altura en 0, 1, o 2 según la elección del usuario 
// e imprime el valor de altura seleccionado
void config_altura()
{
  if(B_altura==1)
  {
    altura++;
    if(altura==3)
      altura=0;
    lcd.setCursor(8, 1);
    switch (altura) {
    case 0:
      //do something when var equals 0
      lcd.print("Bajo ");
      break;
    case 1:
      //do something when var equals 1
      lcd.print("Medio");
      break;
    case 2:
      //do something when var equals 2
      lcd.print("Alto ");
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
    }
  }
}

//**Genera el pwm según la velocidad elegida
void inicio_velocidad()
{
  if(B_aceptar==1 && nivel != -1 && altura != -1)
  {
    //**Genera el pwm según la velocidad elegida
    switch (nivel) {
    case 0:
      //do something when var equals 0
      analogWrite(10,80);
      break;
    case 1:
      //do something when var equals 1
      analogWrite(10,127);
      break;
    case 2:
      //do something when var equals 2
      analogWrite(10,200);
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
    }
  }
}

//**Controla el tiempo de encendido del motor de altura**//
void inicio_altura()
{
  seg = altura-altura_p;
  if(seg!=0)
    timer.disable(wd_timer_id);
    
  if(B_aceptar==1 && nivel != -1 && altura != -1)
  {   
    if(seg!=0)
    {
      intervalo=millis()-t_previo;
      t_previo=millis();
      if(control==false)
      {
      intervalo=14000;     
      control = !control; 
      }
      if(intervalo>14000)
        intervalo=14000;
      if(altura==0) 
      {
        IN_3=true;
        IN_4=false;
        digitalWrite (IN3, IN_3);
        digitalWrite (IN4, IN_4);
		wd_timer_id=timer.setTimeout(14000, apagar);
        timer.enable(wd_timer_id);
      }
      if(altura==2)
      {
        IN_3=false;
        IN_4=true;
        digitalWrite (IN3, IN_3);
        digitalWrite (IN4, IN_4);
        wd_timer_id=timer.setTimeout(14000, apagar);
        timer.enable(wd_timer_id);
      }
      if(altura==1)
      {
        if(altura_pp==1 && intervalo < 7000)
          intervalo=intervalo+7000;
        time_on=7000-intervalo;
        if(time_on > 0)
        {
          wd_timer_id=timer.setTimeout(time_on, apagar);
          timer.enable(wd_timer_id);
        }
        if(time_on < 0)
        {
          time_on=-1*time_on;
          IN_3=!IN_3;
          IN_4=!IN_4;
          digitalWrite (IN3, IN_3);
          digitalWrite (IN4, IN_4); 
          wd_timer_id=timer.setTimeout(time_on, apagar);
          timer.enable(wd_timer_id);
        }
      }      
    }
    altura_pp=altura_p;
    altura_p = altura;    
  }
}

