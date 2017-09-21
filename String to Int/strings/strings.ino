const int En_WrRd_RS485 =  2;

const int Led_1 =  13; 
const int Led_2 =  3; 
const int Led_3 =  4;  

char VarChar = ' ';
String BufferIn = "";  
String NumIn = "";      
boolean StringCompleta = false; 

void setup() 
{ 
  Serial.begin(9600);
  BufferIn.reserve(5);  
    
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
      digitalWrite(En_WrRd_RS485, LOW); 
      Serial.println(BufferIn);
      Serial.println(NumIn);
      
      //if ((BufferIn.indexOf('B')) >= 0)
      if ((BufferIn.indexOf('M')) >= 0)
      {
        int serialValue = (NumIn.toInt());
        serialValue += 3;
        Serial.print(serialValue);
        StringCompleta = false;
        BufferIn = "";
        NumIn = "";
      } 
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
}
