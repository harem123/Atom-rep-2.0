const int En_WrRd_RS485 =  2;
const int Led_1 =  13; 
 
char VarChar = ' ';
String BufferIn = "";  
String NumIn = "";      
boolean StringCompleta = false; 

void setup() 
{ 
  Serial.begin(9600);// inicio serial a 9600 bauds
  BufferIn.reserve(5); // reservo al menos 5 bytes para el buffer

  pinMode(En_WrRd_RS485, OUTPUT);
  pinMode(Led_1, OUTPUT); 
  digitalWrite(En_WrRd_RS485, LOW); 
  digitalWrite(Led_1, LOW);
 
} 
 
void loop() 
{ 
  if (StringCompleta) 
  {         
      delay(50);
      digitalWrite(En_WrRd_RS485, LOW); 
      Serial.println(BufferIn);
      Serial.println(NumIn);
      
      
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
