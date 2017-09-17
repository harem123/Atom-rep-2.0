// motor controlling

// modificado 9 de sep

//sumary el script debe recibir una unica orden de encender motores a baja velocidad 
// luego estar encendido hasta que reciba una unica orden 
// luego leer la velocidad 
// tiene 2 lanzadores entonces va a esperar la orden A para altas y B para bajas 
// segun eso movera un motor reductor que permita al balon caer y luego otro que haga
// que caiga realmente 
// se lee un sensor que indica si el balon ya se disparo  
// 
// pwm pins 2 to 13 and 44 to 46

// ++++++++++ variables de motores  
int m1 = 2,m2=3, m3 = 4, m5 = 5 
int normalVar = 0; // normaliza una orden de 0 a 100 por una de 0 a 255


void setup() 
{
 
  //++++++++++++ defino motores
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
  

}

void loop() 
{
  
    
motorControl(serialValue);
    
  
 
}

void motorControl (int serialValue)
{
  if (serialValue < 101)
    {
      normalVar = serialValue * 2;
      analogWrite(m1,normalVar);
      analogWrite(m2,normalVar);
    }
  else(Serial.print("out of range"))
}
