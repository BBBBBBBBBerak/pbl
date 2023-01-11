#include <SoftwareSerial.h>	// libreria que permite establecer pines digitales para comunicacion serie
#include <TinyGPS.h>

SoftwareSerial miBT(10, 11); 	// pin 10 como RX, pin 11 como TX
SoftwareSerial Serial1(4, 3); //Asignar el puerto a los pines(Al GPS)

TinyGPS gps;
String dato_gps;


void setup() {

  Serial.begin(9600);  
  miBT.begin(38400);		// comunicacion serie entre Arduino y el modulo a 38400 bps
  Serial1.begin(9600);
}

void loop() {  
  miBT.print(GPS());
  Serial.println(GPS());

  delay(1000);
} 

String GPS(){
   
   bool newData = false;  //Funcion para saber si hay dato_gpss
   
   // Intentar recibir secuencia durante un segundo
   for (unsigned long start = millis(); millis() - start < 1000;)
   {
      while (Serial1.available())
      {
         char c = Serial1.read();  //Guarda dentro de c los valores que de el GPS
         if (gps.encode(c))  //Descodifica la nueva secuencia recibida
            newData = true;
      }
   }
 
   if (newData)
    {
      float flat, flon;   //Variables de latitud y longitud
      unsigned long age;
      gps.f_get_position(&flat, &flon, &age);

      //Guardamos en el String todos los dato_gps
      dato_gps=(
      String(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6) + String(" ,  ") + //Latitud
      String(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6) +  String(" ,  "));} //Longitud)
      
     else
       {
          dato_gps = String("---") + String(" ,  ") + String("---") + String(" ,  ");
          }
   
 return dato_gps;
}
