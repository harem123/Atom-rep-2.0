// This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling.

#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "KIKE123";
char wifiPassword[] = "27351989";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "01ebecb0-6819-11e8-827b-4f89eb065ec7";
char password[] = "184b8753d52520171ffbcb394d0beee291c39840";
char clientID[] = "202a73f0-6822-11e8-bcfb-4dffbccea8f6";

unsigned long lastMillis = 2000;
int horas = 0;
int minutos = 1;
int segundos = 0;
bool flag_led = false;
bool flag_pastilla = false;

void setup() {
    
    
    Serial.begin(9600);
    delay(2000);
    Cayenne.begin(username, password, clientID, ssid, wifiPassword);// iba abajo se paso arriba 
    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(3,INPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    //Serial.print(segundos);
   // Serial.println(" ");
    
    
}

void loop() {
  if(digitalRead(3) == LOW)
  {
    flag_pastilla = true;
  }
    if( flag_pastilla == true){
    segundos = 0;
    minutos = 0;
    horas = 0;
  }
  if (millis()-lastMillis > 1000)
  {
     
     segundos++;
     lastMillis = millis();
     flag_led != flag_led;
      if (WiFi.status() == WL_CONNECTED) {
           if (flag_led == true)
           digitalWrite(LED_BUILTIN, HIGH);}   // turn the LED on (HIGH is the voltage level)
           else                       // wait for a second
           {digitalWrite(LED_BUILTIN, LOW);}  // turn the LED off by making the voltage LOW
           }
  
     
     if(segundos > 59 )
     {
        segundos = 0;
        minutos++;
        if (minutos > 59 )
         {
          horas++;
          minutos = 0;
         }
     }
 }

    
    
    Cayenne.loop();
    
  /*  if (WiFi.status() != WL_CONNECTED)
    {
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
        delay(2000);
        WiFi.begin(ssid, password);}
      
       //Cayenne.begin(username, password, clientID, ssid, wifiPassword);
       while (WiFi.status() != WL_CONNECTED)
       {
       delay(500);
       //Serial.print(".");  
       } */
     
 
}
// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
    // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
    Cayenne.virtualWrite(0, horas);
    Cayenne.virtualWrite(1, minutos);
    Cayenne.virtualWrite(2, segundos);
 
    // Some examples of other functions you can use to send data.
    //Cayenne.celsiusWrite(1, 22.0);
    //Cayenne.luxWrite(2, 700);
    //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
    CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
    //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
