
const int En_WrRd_RS485 =  2;

const int Led_1 =  13; 
const int Led_2 =  6; 
const int Led_3 =  5;  
// ++ pines de sensado my pins y pin de salida 
int myPins[] = {22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53};
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
  for(int j= 0 ; j <32; j++){
  pinMode(myPins[j],INPUT);}
// +++

  Serial.begin(9600);
  BufferIn.reserve(6);  
    
  pinMode(En_WrRd_RS485, OUTPUT);
  
  pinMode(Led_1, OUTPUT);
  
  
  digitalWrite(En_WrRd_RS485, LOW); 
  
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
            delay(50);
            currentSec = millis();
            previousSec = currentSec; // inicio temporizadores
            while(flag == true && (currentSec - previousSec) < 5000)
            {  

              digitalWrite(Led_1, HIGH);
              for(int i = 0; i<31; i++)   // ciclo de 5 ms para 32 celdas 
              {
                if (digitalRead(myPins[i]) == HIGH)
                {
                  sN = i;
                  digitalWrite(Led_1, LOW);
                  Serial.print("L");
                  Serial.print("T");
                  Serial.print(sN);
                  Serial.print("Z");  
                  i=31;// cambia el estado para salir rapido del for 
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



