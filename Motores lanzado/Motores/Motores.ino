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
const int m1 = 5, m2=6;
const int lockA1 = 22,lockA2 = 23,lockA3 = 24,lockA4 = 25; // PINES MOTORES LOCK LANZADOR 1
const int lockB1 = 26,lockB2 = 27,lockB3 = 28,lockB4 = 29; // PINES MOTORES LOCK LANZADOR 1
const int lockC1 = 30,lockC2 = 31,lockC3 = 32,lockC4 = 33; // PINES MOTORES LOCK LANZADOR 2
const int lockD1 = 34,lockD2 = 35,lockD3 = 36,lockD4 = 37; // PINES MOTORES LOCK LANZADOR 2
int normalVar = 0; // normaliza una orden de 0 a 100 por una de 0 a 255


void setup() 
{
 
  //++++++++++++ defino motores
  pinMode(m1, OUTPUT);pinMode(m2, OUTPUT);
  pinMode(lockA1, OUTPUT);pinMode(lockA2, OUTPUT);pinMode(lockA3, OUTPUT);pinMode(lockA4, OUTPUT);
  pinMode(lockB1, OUTPUT);pinMode(lockB2, OUTPUT);pinMode(lockB3, OUTPUT);pinMode(lockB4, OUTPUT);
  pinMode(lockC1, OUTPUT);pinMode(lockC2, OUTPUT);pinMode(lockC3, OUTPUT);pinMode(lockC4, OUTPUT);
  pinMode(lockD1, OUTPUT);pinMode(lockD2, OUTPUT);pinMode(lockD3, OUTPUT);pinMode(lockD4, OUTPUT);

}

void loop() 
{
  
char serialValue= "l"; 
char lanzadorNumber = "a";   
motorControl(serialValue, lanzadorNumber); // recibe el valor pwm de 0 a 100 y tiro alto o bajo 
    
  
 
}

void motorControl (int serialValue, char lanzadorId)
{
  int speed;
  switch (serialValue)
  {
    case 1: speed = 70; break;
    case 2: speed = 140; break;
    case 3: speed = 210; break;
    default: Serial.print("error speed");break; // falta habilitar RS 485
  }
  analogWrite(m1,speed);
  analogWrite(m2,speed); 
  //lockA1 = 22,lockA2 = 23,lockA3 = 24,lockA4 = 25;
  if (lanzadorId == "a")
  {
// +++++++ INICIO AVANCE DE MOTOR L1
   
    unlocker(lockA1,lockA2,3000);
// ++++++++++ INICIO REGRESO DE MOTOR L1
   
    unlocker(lockA3,lockA4,3000);
// +++++++ INICIO AVANCE DE MOTOR L2
    
    unlocker(lockB1,lockB2,3000);
// ++++++++++ INICIO REGRESO DE MOTOR L2
    
    unlocker(lockB3,lockB4,3000);
  }
  if (lanzadorNumber == "b")
  {

// +++++++ INICIO AVANCE DE MOTOR L1
   
    unlocker(lockC1,lockC2,3000);
// ++++++++++ INICIO REGRESO DE MOTOR L1
   
    unlocker(lockC3,lockC4,3000);
// +++++++ INICIO AVANCE DE MOTOR L2
    
    unlocker(lockD1,lockD2,3000);
// ++++++++++ INICIO REGRESO DE MOTOR L2
    
    unlocker(lockD3,lockD4,3000);

  }
}

void unlocker(int motor1,int motor2, int time)
{
    digitalWrite(motor1, HIGH);// ABRE CAMINO DE PUENTE H
    digitalWrite(motor2, HIGH);
    delay(time);// PERMITE GIRAR AL MOTOR SEGUN EL CAMINO DE PUENTE H
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, LOW);
    delay(500);
}

