/*---------------------------------------*
* Made by: Jon Caballero,               *
*          Iker Arenaza,                *
*          Igor Bikandi,                *
*          Eneko Begiristain,           *
*          Jokin Bengoa,                *
*          Ander Beracoechea.           *
*                                       *
* Name: Prueba_v1_pbl                   *
*                                       *
*                                       *
*--------------------------------------*/

#include <SoftwareSerial.h>	// libreria que permite establecer pines digitales para comunicacion serie
#include <TinyGPS.h>
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();

SoftwareSerial miBT(10, 11); 	// pin 10 como RX, pin 11 como TX
SoftwareSerial Serial1(4, 3); //Asignar el puerto a los pines(Al GPS)

TinyGPS gps;
String dato_gps;


void setup() {
  Serial.begin(9600);  
  miBT.begin(38400);		// comunicacion serie entre Arduino y el modulo a 38400 bps
  Serial1.begin(9600);

  Serial.println("Adafruit MMA8451 test!");

  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  mma.setRange(MMA8451_RANGE_2_G);
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");
}

void loop() {  
  miBT.print(GPS());
  Serial.print("GPS -->");
  Serial.println(GPS());
  Serial.print("ACELEROMETRO -->");
  Serial.println(aceleracion());
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
 
      float flat, flon;   //Variables de latitud y longitud
      unsigned long age;
      gps.f_get_position(&flat, &flon, &age);

      //Guardamos en el String todos los dato_gps
      dato_gps=(
      String(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6) + String(" ,  ") + //Latitud
      String(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6) +  String(" ,  ")); //Longitud
         
 return dato_gps;
}

float aceleracion(){
  sensors_event_t event; 
  mma.getEvent(&event);
  float x=event.acceleration.x;
  float y=event.acceleration.y;
  float z=event.acceleration.z;
  float mod=sqrt(pow(x,2)+pow(y,2)+pow(z,2));
  return mod;
}