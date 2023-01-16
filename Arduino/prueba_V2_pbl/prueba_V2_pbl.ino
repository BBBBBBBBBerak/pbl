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


const int ledPin = 7;// the number of the LED pin
const int pulsador = 8;//input pullup del pulsador
int ledState = LOW; // ledState used to set the LED
unsigned long previousMillis = 0; // will store last time LED was updated
unsigned long currentMillis = 0; // will store current time LED
int estado = 0;
int BUZZERSTATE = 0;
int contador = 0;


Adafruit_MMA8451 mma = Adafruit_MMA8451();

SoftwareSerial miBT(10, 11); 	// pin 10 como RX, pin 11 como TX
SoftwareSerial Serial1(4, 3); //Asignar el puerto a los pines(Al GPS)

TinyGPS gps;
String dato_gps;
String datoGPS;
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pulsador, INPUT_PULLUP);
  Serial.begin(9600);   // Serial pc arduino
  miBT.begin(38400);		// comunicacion serie entre Arduino y el modulo a 38400 bps
  Serial1.begin(9600);  // GPS

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
  datoGPS = GPS();
  //Serial.println("GPS!");
  digitalWrite(ledPin, LOW);
  //noTone(6);
  //miBT.print(GPS());  
  //Serial.print("GPS -->");
  //Serial.println(GPS());
  //Serial.print("ACELEROMETRO -->");
  //Serial.println(aceleracion());

  if(miBT.available() > 0 || digitalRead(pulsador)== LOW){
    Serial.println("Pulsador pulsado");
    estado = 2;
  }

  switch (estado){
  case 1:
    digitalWrite(ledPin, LOW);
    noTone(6); 
  break;

  case 2:
    if(contador == 0){

      miBT.print(datoGPS);
      contador++;
    }
    currentMillis = millis(); //take the current time
    if (currentMillis - previousMillis >= 1000) {
    // save the last time you blinked the BUZZER
    previousMillis = currentMillis;

    // if the BUZZER is off turn it on and vice-versa:
    if (BUZZERSTATE == LOW) {
      BUZZERSTATE = HIGH;
      tone(6, 200, 1000);
      ledState = HIGH;
    } else {
      BUZZERSTATE = LOW;
      noTone(6);
      ledState = LOW;
    }
      // set the LED with the ledState of the variable:
    digitalWrite(ledPin, HIGH);
    }
    /*if (currentMillis - previousMillis >= 1000 && ledState==LOW) {
      digitalWrite(ledPin, HIGH);
      tone(6, 500);
      previousMillis = currentMillis;
    }
    else{
    digitalWrite(ledPin, LOW);
    noTone(6);
    }*/
    if(miBT.available() > 0 || !digitalRead(pulsador)){
    estado = 2;
  }
  break;
  }
}

String GPS(){   
   // Intentar recibir secuencia durante un segundo
   //for (unsigned long start = millis(); millis() - start < 1000;)
   {
      while (Serial1.available())
      {
         char c = Serial1.read();  //Guarda dentro de c los valores que de el GPS
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