#include <Colorduino.h>
//#include <SimpleTimer.h>

int x=0; //Filas de celda verde
int y=0; //Columnas de celda verde
int x2=0; //Filas de celda azul
int y2=1; //Columnas de celda azul
int x3=0; //Filas de celda dorada
int y3=1; //Columnas de celda dorada
int y_verde=5; //Almacena valor de columna para celda verde
int y_rojo1=0; //Almacena valor de columna para celda rojo anterior
int y_rojo2=0; //Almacena valor de columna para celda rojo posterior
int x_verde=0; //Almacena valor de fila para celda verde 
int puntaje=0; //Almacena el valor del puntaje de cada anotación
unsigned int M_rojo[8][8]; //Matriz color rojo
unsigned int M_verde[8][8]; //Matriz color verde
unsigned int M_azul[8][8]; //Matriz color azul
unsigned int cont2=0; //Contador para determinar la cantidad de veces que se repite el juego antes de terminar
unsigned int cont=0; //Contador para la función Celdas
long currentmillis=0;
long previousmillis=0;
//SimpleTimer timer;
//int wd_timer_id; //Control de timer
boolean ctrol = false;
boolean flag_enable1=false;
boolean flag_enable2=false;
boolean flag_colorear=false;

//**variables para conversion de valor serial recibido a filas y columnas**//
int xs;
int ys;

//**Variables para transmisión serial**//
const int En_WrRd_RS485 =  22;
const int Led_1 =  13; 
 
char VarChar = ' ';
String BufferIn = "";  
String NumIn = "";      
boolean StringCompleta = false; 

void setup() {
  
  Serial.begin(9600);// inicio serial a 9600 bauds
  BufferIn.reserve(5); // reservo al menos 5 bytes para el buffer
  pinMode(En_WrRd_RS485, OUTPUT);
  //digitalWrite(En_WrRd_RS485, LOW); 
  digitalWrite(En_WrRd_RS485, HIGH);
  delay(100);
  Serial.println("IN");
  delay(100);
  digitalWrite(En_WrRd_RS485, LOW);
  
  Colorduino.Init();
  unsigned char whiteBalVal[3] = {36,63,63}; // for LEDSEE 6x6cm round matrix
  Colorduino.SetWhiteBal(whiteBalVal);
  
  //wd_timer_id=timer.setInterval(1000, Celdas);
  //timer.disable(wd_timer_id);
}

void loop() {
  
  //timer.run();
  currentmillis=millis()-previousmillis;

  if(flag_enable1==true)
  {
    if(flag_enable2==true)
    {
      currentmillis=3001;
      flag_enable2=false;
    }
    if(currentmillis > 1000)
      {
        previousmillis=millis();
        Celdas();
      }
  }
  if(cont==6)//Modifica para cambiar las veces que la celda verde se desplaza así: cont == (veces a desplazar)+1, también modificar el wd_timer
  { 
    apagartodo();
    cont=0;
    flag_enable2=false;
    flag_enable1=false;
  }

  if(cont2==18)//Modifica para cambiar las veces que se repite el juego así: cont2 == 6*(veces a repetir)
  {
    digitalWrite(En_WrRd_RS485, HIGH);
    delay(100);
    Serial.print("Puntaje: ");
    delay(100);
    Serial.print(puntaje);
    delay(100);
    digitalWrite(En_WrRd_RS485, LOW);
    //delay(50);
    apagartodo();
    cont=0;
    cont2=0;
    puntaje=0;
  }
}

//**Funcion Leer_M recorre las matrices de colores 
void Leer_M()
{
  int x_m;//filas
  int y_m;//columnas
  for(x_m=0;x_m<8;x_m++)
  {
    for(y_m=0;y_m<8;y_m++)
    {
      Colorduino.SetPixel(x_m,y_m,M_rojo[x_m][y_m],M_verde[x_m][y_m],M_azul[x_m][y_m]);//rgb
    }    
  }
}

//**Funcion Celdas se activa cada 3 segundos y controla el encendido de las celdas verde, dorada y azul
void Celdas()
{    
   //*Configura celdas rojo-verde-rojo*// 
    M_rojo[x_verde][y_rojo1]=0;
    M_rojo[x_verde][y_rojo2]=0;
    M_verde[x_verde][y_verde]=0;
    
    if(cont==0)
    {
      /*
      randomSeed(analogRead(A5));
      x=random(0, 6);  
      randomSeed(analogRead(A4));
      y_verde=random(1, 6);
      y=y_verde;*/
      x=2;
      y=1;
    }
    
    x_verde=x;
    y_verde=y;
    y_rojo1=y-1;
    y_rojo2=y+1;
    M_rojo[x][y_rojo1]=255;
    M_rojo[x][y_rojo2]=255;
    M_verde[x][y]=255;
    if(y==6)
    {
      y=1;
      x=x+2;
      if(x==8)
        x==0;
      if(x==9)
        x==1;
    }
    else
    {
      y++;
    }
   //*Fin configuracion celdas rojo-verde-rojo*//
   
   //*Configura celda azul*// 
    if(cont==0)
    {
      M_azul[x2][y2]=0;
      /*do
      {
        x2=random(0, 7);
      } while (x2==x_verde || x2==x_verde+2);
      y2=random(0, 7);*/
      x2=0;
      y2=5;
      M_azul[x2][y2]=255;
      ctrol = false;
      
    }
   //*Fin configuración celda azul*//
   
   //*Configura celda dorada*// 
    if(ctrol==true)
    {
      M_rojo[x3][y3]=0;
      M_verde[x3][y3]=0;
      M_azul[x3][y3]=0;
      x3=70;
      y3=70;
    }
    if(ctrol==false)
    {
      /*do
      {
        x3=random(0, 7);
      } while (x3==x_verde);
      y3=random(0, 7);*/
      x3=0;
      y3=2;
      M_rojo[x3][y3]=212;
      M_verde[x3][y3]=175;
      M_azul[x3][y3]=55;
      ctrol = !ctrol; 
    }
    //*Fin configuracion celda dorada//
    if(flag_colorear==true)
    {
      Leer_M();//Carga valores a la matriz colorduino
      Colorduino.FlipPage();//Enciende la matriz
      cont++;      
      cont2++;
    } 
    //Serial.println(cont2);   
//    Serial.println(cont1);   
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

  //*Instrucciones a realizar si se recibió datos por el puerto serial*//
  if (StringCompleta) 
  {      
      delay(50);
      digitalWrite(En_WrRd_RS485, LOW); 
      //Serial.println(BufferIn);
      //Serial.println(NumIn);
           
      if ((BufferIn.indexOf('L')) >= 0)
      {
        apagartodo(); 
          
        flag_enable2=false;
        flag_enable1=false;
        //Comprueba si se recibe letra I para iniciar el juego
        if ((BufferIn.indexOf('I')) >= 0)
        {
          //wd_timer_id=timer.setTimer(1000, Celdas, 6);//Modificar para cambiar las veces que se desplaza la celda verde (n_veces+1) y el tiempo de cada ronda
          flag_enable2=true;
          flag_enable1=true;
          flag_colorear=true;
        }
        //Comprueba si se recibe letra T para terminar y determinar si la anotación es correcta
        if ((BufferIn.indexOf('T')) >= 0)
        {
          cont2=cont2+6-cont;
          cont=6;
          int serialValue = (NumIn.toInt());
          xs = serialValue/8;
          ys = serialValue - 8*xs;
          flag_colorear=false;
          if(xs==x_verde && ys==y_verde)
          {
            puntaje=puntaje+2;          
          }
          if(xs==x2 && ys==y2)
          {
            puntaje++;
          }
          if(xs==x3 && ys==y3)
          {
            puntaje=puntaje+3;
          }
          y_rojo1=y_verde-1;
          y_rojo2=y_verde+1;
          if(xs==x_verde && ys==y_rojo1 || xs==x_verde && ys==y_rojo2)
          {
            puntaje=puntaje-1;
          }
          digitalWrite(En_WrRd_RS485, HIGH);
          delay(100);  
          Serial.println(puntaje); 
          delay(100);
          digitalWrite(En_WrRd_RS485, LOW); 
              
        }
      }
    //Reinicia variables
    StringCompleta = false;
    BufferIn = "";
    NumIn = "";     
  }
}

void apagartodo()
{
  for(int k=0; k<8; k++)
  {
    for(int j=0; j<8; j++)
    {
      M_rojo[k][j]=0; //Matriz color rojo en 0
      M_verde[k][j]=0; //Matriz color verde en 0
      M_azul[k][j]=0; //Matriz color azul en 0
    }
  }
  Leer_M();
  Colorduino.FlipPage();
  delay(70);
}

