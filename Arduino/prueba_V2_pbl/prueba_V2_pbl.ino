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
float dato_aceleracion = 9.8;
int estado_led = LOW;

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
  dato_aceleracion = aceleracion();

  if(dato_aceleracion > 17){
    estado = 2;
  }
  switch (estado){
  case 1:
    digitalWrite(ledPin, LOW);
    Serial.println("Caso 1");
    estado_led = LOW;
    noTone(6); 
    datoGPS = GPS();
    dato_aceleracion = aceleracion();
    
    if(dato_aceleracion > 17){
      estado = 2;
    }

  break;

  case 2:
    //Serial.println("Caso 2");
//    digitalWrite(ledPin, estado_led);
    estado_led = !estado_led;
    if(contador == 0){
      miBT.print(datoGPS);
      contador++;
    }
    currentMillis = millis(); //take the current time
    if (currentMillis - previousMillis >= 1000) { // save the last time you blinked the BUZZER

    // if the BUZZER is off turn it on and vice-versa:
    if (BUZZERSTATE == LOW) {
      BUZZERSTATE = HIGH;
      digitalWrite(ledPin, HIGH);
      estado_led = !estado_led;
      tone(6, 500);
      } 
      
      else {
      BUZZERSTATE = LOW;
      noTone(6);
      digitalWrite(ledPin, LOW);
    }
    previousMillis = currentMillis;
    }
    
    if(miBT.available() > 0 || digitalRead(pulsador)== LOW){
    estado = 1;
    }
  break;
  }
}

String GPS(){ 
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