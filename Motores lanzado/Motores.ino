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
const int m1 = 2, m2=3;
const int lockA1 = 22,lockA2 = 23,lockA3 = 24,lockA4 = 25; // PINES MOTORES LOCK LANZADOR 1
const int lockB1 = 26,lockB2 = 27,lockB3 = 28,lockB4 = 29; // PINES MOTORES LOCK LANZADOR 1
const int lockC1 = 30,lockC2 = 31,lockC3 = 32,lockC4 = 33; // PINES MOTORES LOCK LANZADOR 2
const int lockD1 = 34,lockD2 = 35,lockD3 = 36,lockD4 = 37; // PINES MOTORES LOCK LANZADOR 2
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
  
    
motorControl(serialValue, lanzadorNumber); // recibe el valor pwm de 0 a 100 y tiro alto o bajo 
    
  
 
}

void motorControl (int serialValue; char lanzadorNumber;)
{
  int speed;
  switch (serialValue){
    case 1: speed = 70 break;
    case 2: speed = 140 break;
    case 3: speed = 210 break;
    default: Serial.print("error speed") // falta habilitar RS 485
  }
  analogWrite(m1,speed);
  analogWrite(m2,speed); 
  //lockA1 = 22,lockA2 = 23,lockA3 = 24,lockA4 = 25;
  if (lanzadorNumber == "a")
  {
// +++++++ INICIO AVANCE DE MOTOR L1
    digitalWrite(lockA1, HIGH);
    digitalWrite(lockA2, HIGH);
    delay(3000);
    digitalWrite(lockA1, LOW);
    digitalWrite(lockA2, LOW);
    delay(500);
// ++++++++++ INICIO REGRESO DE MOTOR L1
    digitalWrite(lockA3, HIGH);
    digitalWrite(lockA4, HIGH);
    delay(3000);
    digitalWrite(lockA3, LOW);
    digitalWrite(lockA4, LOW);
  }
  if (lanzadorNumber == "b")
  {
    digitalWrite(lockB1, HIGH);
    digitalWrite(lockB2, HIGH);
    delay(3000);
    digitalWrite(lockB1, LOW);
    digitalWrite(lockB2, LOW);
    delay(500);
// ++++++++++ INICIO REGRESO DE MOTOR L1
    digitalWrite(lockB3, HIGH);
    digitalWrite(lockB4, HIGH);
    delay(3000);
    digitalWrite(lockB3, LOW);
    digitalWrite(lockB4, LOW);
  }
}
