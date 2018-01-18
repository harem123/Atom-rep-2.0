
const int En_WrRd_RS485 =  2;

const int Led_1 =  13; 
const int Led_2 =  6; 
const int Led_3 =  5;  
// ++ pines de sensado my pins y pin de salida 
int myPins[] = {7, 8,9};
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
  //++ defino pines de entrada 
  for(int j= 0 ; j <2; j++){
  pinMode(myPins[j],INPUT);}
// +++

  Serial.begin(9600);
  BufferIn.reserve(6);  
    
  pinMode(En_WrRd_RS485, OUTPUT);
  
  pinMode(Led_1, OUTPUT);
  pinMode(Led_2, OUTPUT);
  pinMode(Led_3, OUTPUT);
  
  digitalWrite(En_WrRd_RS485, LOW); 
  digitalWrite(Led_1, LOW);
  digitalWrite(Led_2, LOW);
  digitalWrite(Led_3, LOW); 
} 
 
void loop() 
{ 
  if (StringCompleta) 
  {         
      delay(50);
      digitalWrite(En_WrRd_RS485, LOW); // pongo pin en LOW para habilitar RS 485
      Serial.print(BufferIn);
      
      if ((BufferIn.indexOf('S')) >= 0) // leo el identificador de la placa sensores
      {
          if (BufferIn.indexOf('1' ) >= 0) // leo el comando de inicio de sensado
          {
            flag = true;                    // permito ingresar al while 
            Serial.println("OK");
            delay(100);
            currentSec = millis();
            previousSec = currentSec; // inicio temporizadores
            while(flag == true && (currentSec - previousSec) < 5000)
            {  

              digitalWrite(Led_2, HIGH);
              for(int i = 0; i<3; i++)   // ciclo de 5 ms para 32 celdas 
              {
                if (digitalRead(myPins[i]) == HIGH)
                {
                  sN = i;
                  digitalWrite(Led_2, LOW);
                  Serial.print("C");
                  Serial.print(sN);
                  Serial.print("#");  
                  i=4;// cambia el estado para salir rapido del for 
                  flag = false;
                }
              }
              currentSec = millis();
              
            }         
            delay(500);
                   
          }       
      }
      StringCompleta = false;
      BufferIn = "";
  }
  else{StringCompleta = false;} 
} 

void serialEvent() {
  while (Serial.available()) 
  {
    VarChar = (char)Serial.read();
    BufferIn += VarChar;
    if (VarChar == 'Z') { StringCompleta = true; }   
  }
}



