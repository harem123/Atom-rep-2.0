#include <SoftwareSerial.h> // GSM 
//#include <LiquidCrystal.h>
//#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
// Inicialización de libreria con los pines lcd

// ****I2C

byte dir = 0x3F;
LiquidCrystal_I2C             lcd( dir, 2, 1, 0, 4, 5, 6, 7);


//LiquidCrystal lcd(22, 23, 24, 25, 26, 27);

SoftwareSerial SIM900(10, 11); // Configura el puerto serial para el SIM900. Para el Arduino MEGA utilizar pines 10 y 11

//variables para configuración e inicio de sim900
int respuesta; 
int x=0; 
char incoming_char = 0; //Variable para guardar los caracteres que envia el SIM900

const int pin_rele = 8; //Pin de activación rele !! se cambia a pin 8 era pin 7

//Variables para almacenamiento de información
String mensaje = ""; 
String telefono = ""; //Guarda celular de quien llama
String nombre = ""; //Guarda nombre de quien llama
String contactos_sim = ""; //Guarda lista de contactos de la simcard

//Variables de control para extracción de datos de llamada entrante
int pos=0; 
int trama = 0;

bool flag_llamada = false; //Habilita impresión de datos de llamada entrante
bool flag_nombre = false; //False Desconocido, True Dato simcard
bool flag_tono = false; //Indica llamada entrante para ejecutar una acción
bool flag_millis = false; //Habilita conteo de 60 seg
bool flag_rele = true; //Estado rele

//Uso función millis
//unsigned long currentmillis=0;
unsigned long previousmillis = 0;
unsigned long previousmillis2 = 0;

byte in[8] =
 {
    0b00000000,     // Los definimos como binarios 0bxxxxxxx
    0b00001100,
    0b00000110,
    0b00011111,
    0b00011111,
    0b00000110,
    0b00001100,
    0b00000000
 };

 byte off[8] =
 {
    0b00000000,     // Los definimos como binarios 0bxxxxxxx
    0b00000000,
    0b00000000,
    0b00011100,
    0b00011000,
    0b00010101,
    0b00000010,
    0b00000000
 };
 
void setup()
{
  SIM900.begin(19200); //Configura velocidad del puerto serie para el SIM900
  Serial.begin(19200); //Configura velocidad del puerto serie del Arduino
  delay(1000);

  lcd.begin (16,2);   
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  
  //lcd.begin(16, 2); //Configura lcd de 16 columnas x 2 filas
  lcd.createChar(1, in);
  lcd.createChar(2, off);
  //Serial.println("Iniciando...");
  lcd.home();
  lcd.setCursor(0, 0); 
  lcd.print("Iniciando..."); 
  
  //Funciones de inicio
  //power_on();
  //iniciar();
  
  //Iniciar y apagar el relé
  pinMode(pin_rele, OUTPUT);
  digitalWrite(pin_rele, HIGH);
  delay(100);
 
  //agenda_sim(); //Extrae los datos de contacto alojados en la sim y almacena en contactos_sim  
}

void loop()
{
  if(1){   //  SIM900.available()) {
    //incoming_char = SIM900.read(); //Guardamos el carácter del GPRS      
    //mensaje = mensaje + incoming_char;
    //identifica_llamada(); //Extrae datos de llamada entrante, almacena en telefono y nombre
    
    if(digitalRead(52)== HIGH);            // flag_tono)** //Ejecuta acción
    {
      Serial.println("HIGH ")
      if(0)   //nombre == "Desconocido") //Cuelga llamada desconocida
        enviarAT("ATH", "OK", 500);
      else
      {      
        if(flag_rele) 
        {
          digitalWrite(pin_rele, LOW);
          flag_rele = false;
          flag_millis = true;
          Serial.println("rele = false ")
          previousmillis = millis();
        }
      }
      flag_tono = false; 
    }
  }
  
  /*if(mensaje.indexOf("NO CARRIER") > 0)
  {
    flag_rele = true;
    lcd.setCursor(0, 0); 
    lcd.write(2);
    mensaje = "";
  }
*/
  if(flag_millis == true && (millis()-previousmillis > 90000)) //Conteo 
  {
    digitalWrite(pin_rele, HIGH);
    flag_millis = false;
  }
  
  /*if(SIM900.available() < 1 && (millis()-previousmillis2 > 60000))
  {
    previousmillis2 = millis();
    if (enviarAT("AT+CREG?", "+CREG: 0,5", 1000) != 1)
    {
      lcd.clear();
      
      reiniciar();
      iniciar();
    }
    //Serial.println("señal");
    delay(100);
  }*/
}

// ************* finaliza loop principal

//Función para el manejo de comando AT
int enviarAT(char* ATcommand, char* resp_correcta, unsigned int tiempo) 
{

  int x = 0;
  bool correcto = 0;
  char respuesta[100];
  unsigned long anterior;

  memset(respuesta, '\0', 100); // Inicializa el string
  delay(100);
  while ( SIM900.available() > 0) SIM900.read(); // Limpia el buffer de entrada
  SIM900.println(ATcommand); // Envia el comando AT
  x = 0;
  anterior = millis();
  // Espera una respuesta
  do {
    // si hay datos el buffer de entrada del UART lee y comprueba la respuesta
    if (SIM900.available() != 0)
    {
      //Comprueba que no haya desbordamiento en la capacidad del buffer
      if (x < 99) {
        respuesta[x] = SIM900.read();
        x++;
      }
      else Serial.println("Desbordamiento!");
      // Comprueba si la respuesta del modulo es la 1
      if (strstr(respuesta, resp_correcta) != NULL)
      {
        correcto = 1;
      }
    }
  }
  // Espera hasta tener una respuesta
  while ((correcto == 0) && ((millis() - anterior) < tiempo));
  //Serial.println(respuesta);
  mensaje = respuesta;
  return correcto;
}

/*void power_on() //Enciende el modulo
{
  int respuesta = 0;

  // Comprueba que el modulo SIM900 esta arrancado
  if (enviarAT("AT", "OK", 2000) == 0) //comprueba la conexion a la red
  {
    Serial.println("Encendiendo el GPRS...");

    pinMode(9, OUTPUT);
    digitalWrite(9, HIGH);
    delay(1000);
    digitalWrite(9, LOW);
    delay(1000);

    // Espera la respuesta del modulo SIM900
    while (respuesta == 0) 
    {
      // Envia un comando AT cada 2 segundos y espera la respuesta
      respuesta = enviarAT("AT", "OK", 2000);
      SIM900.println(respuesta);
    }
  }
}

void power_off() //Apaga el modulo
{
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(1000);
}

void reiniciar() //Reinicia el modulo 
{
  Serial.println("Reiniciando...");
  power_off();
  delay (5000);
  power_on();
}

void iniciar() //Conecta el modulo a la red
{
  enviarAT("AT+CPIN=\"8876\"", "OK", 1000); // numero 3017513345
  Serial.println("Conectando a la red...");
  delay (5000);
  lcd.setCursor(0, 0); 
  lcd.print("Conectando red..."); 
  //espera hasta estar conectado a la red movil
  while ( enviarAT("AT+CREG?", "+CREG: 0,5", 1000) == 0 )
  {
  }
  Serial.println("Conectado a la red.");
  enviarAT("AT+CLIP=1\r", "OK", 1000); // Activamos la identificacion de llamadas
  Serial.println("Preparado.");
  lcd.setCursor(0, 0); 
  lcd.print("Preparado       "); 
}
*/

 //Extrae datos de llamada entrante, almacena en telefono y nombre
void identifica_llamada()
{
  if(mensaje.indexOf("+CLIP:") > 0)
  {
     trama = 1;
     pos = 0;
     mensaje = ""; 
     telefono = "";
     nombre = "";
  }    
  if(trama == 1 && (mensaje.indexOf("3") > 0))
    trama = 2;
  if(trama == 2)
  {
    telefono = telefono + incoming_char;
    pos++;
    if(pos == 10)
    {
      trama = 3;
    }
  }
  if(trama == 5)
  {
    if(incoming_char == '"')
    {
      trama = 0;
      flag_llamada = true;
      if(flag_nombre == false)
      {
        nombre = "Desconocido";        
      }
      flag_nombre = false;
    }
    else
    {
      nombre = nombre + incoming_char;
      flag_nombre = true;
    }
  }
  if(trama == 4)
    trama = 5;
  if(trama == 3 && (mensaje.indexOf(",,") > 0)) 
    trama = 4;  
  if(flag_llamada)
  {
    lcd.clear();
    lcd.setCursor(0, 0); 
    lcd.write(1);  
    lcd.setCursor(2, 0);
    //Serial.println("RING ");
    //Serial.print(nombre);
    //Serial.print(" ");
    //Serial.print(telefono);
    //Serial.println("");
    lcd.print(nombre);
    lcd.setCursor(0, 1);
    lcd.print(telefono);
    flag_llamada = false;
    flag_tono = true;
    delay(100);
  }
}

//Extrae los datos de contacto alojados en la sim y almacena en contactos_sim  
void agenda_sim() 
{
  SIM900.println("AT+CPBR=1");
  delay(100);
  while (SIM900.available()) 
  {
    incoming_char = SIM900.read(); //Guardamos el carácter del GPRS
    contactos_sim = contactos_sim + incoming_char;
  }
}

