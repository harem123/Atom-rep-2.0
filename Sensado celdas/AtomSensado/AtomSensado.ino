
const int En_WrRd_RS485 =  2;

const int Led_1 =  13; 
 
// ++ pines de sensado my pins y pin de salida 
int myPins[] = {47,48,49,50,51,52,53};//22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,
int sN=00;
boolean flag;
// ++++++++++++++++++++++++++++++
// variables de buffering
char VarChar = ' ';
String BufferIn = "";        
boolean StringCompleta = false; 

// variables temporizador
unsigned long currentSec; 
unsigned long previousSec;

void setup() 
{ 
  //defino pines de entrada 
 for(int j= 0 ; j <7; j++){
 pinMode(myPins[j],INPUT);}
// +++

  Serial.begin(9600);
  BufferIn.reserve(6);  
    
  pinMode(En_WrRd_RS485, OUTPUT);
  
  pinMode(Led_1, OUTPUT);
  digitalWrite(Led_1, HIGH);
  digitalWrite(En_WrRd_RS485, HIGH);
  delay(100);
  Serial.println("SETUP");
  delay(100);
  digitalWrite(En_WrRd_RS485, LOW);
          
  
 
  
} 
 
void loop() 
{ 
  if (StringCompleta) 
  {         
      delay(50);
      digitalWrite(En_WrRd_RS485, HIGH); 
      delay(50);
      Serial.print(BufferIn);
      delay(50);
      digitalWrite(En_WrRd_RS485, LOW);
      delay(50);
      
      if ((BufferIn.indexOf('S')) >= 0) // leo el identificador de la placa sensores
      {
          if (BufferIn.indexOf('1' ) >= 0) // leo el comando de inicio de sensado
          {
            flag = true;   
            digitalWrite(En_WrRd_RS485, HIGH);
            delay(50);                 // permito ingresar al while 
            Serial.println("Z RECIBIDO");
            delay(50);
            digitalWrite(En_WrRd_RS485, LOW);
            
  
            currentSec = millis();
            previousSec = currentSec; // inicio temporizadores
            while(flag == true && (currentSec - previousSec) < 10000)
            {  

                  digitalWrite(En_WrRd_RS485, HIGH);
            delay(50);                 // permito ingresar al while 
            Serial.println("Z RECIBIDO");
            delay(50);
            digitalWrite(En_WrRd_RS485, LOW);
              
              for(int i = 0; i<7; i++)   // ciclo de 5 ms para 32 celdas 
              {
                if (digitalRead(myPins[i]) == HIGH)
                {
                  sN = i;
                  i=31;// cambia el estado para salir rapido del for 
                  digitalWrite(Led_1, LOW);
                  digitalWrite(En_WrRd_RS485, HIGH);
                  delay(100);
                  Serial.print("LT");
                  delay(50);
                  Serial.print(sN);
                  delay(50);
                  Serial.print("Z");  
                  delay(50);
                  digitalWrite(En_WrRd_RS485, LOW);
                  sN=9;
                  flag = false;
                }
              }
              currentSec = millis();  
            }         
          }       
      }
      StringCompleta = false;
      BufferIn = "";
            /*digitalWrite(En_WrRd_RS485, HIGH);
            delay(50);                
            Serial.print("LT");
            delay(50);
            Serial.print(sN);
            delay(50);
            Serial.print("Z");
            delay(100);
            digitalWrite(En_WrRd_RS485, LOW);*/
            sN=9;
            
  }
  else{StringCompleta = false;} 
} 

void serialEvent() {
  while (Serial.available()) 
  {
    
    VarChar = (char)Serial.read();
    BufferIn += VarChar;
    if (VarChar == 'Z') { 
      StringCompleta = true;
      digitalWrite(Led_1,LOW);
        }   
  }
}



