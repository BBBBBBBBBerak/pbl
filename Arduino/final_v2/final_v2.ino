/*---------------------------------------*
* Made by: Jon Caballero,               *
*          Iker Arenaza,                *
*          Igor Bikandi,                *
*          Eneko Begiristain,           *
*          Jokin Bengoa,                *
*          Ander Beracoechea.           *
*                                       *
* Name: final_v1                        * 
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
unsigned long previousMillis = 0;
unsigned long previousMillisK1 = 0;
unsigned long previousMillisK2 = 0;
unsigned long K2 = 0; // will store last time LED was updated
unsigned long currentMillis = 0; // will store current time LED
int estado = 0;
int BUZZERSTATE = 0;
int contador = 0;

bool kondizioa1 = false;
bool kondizioa2 = false;
bool kondizioa3 = false;

Adafruit_MMA8451 mma = Adafruit_MMA8451();

SoftwareSerial miBT(10, 11); 	// pin 10 como RX, pin 11 como TX
SoftwareSerial Serial1(4, 3); //Asignar el puerto a los pines(Al GPS)

TinyGPS gps;
String dato_gps;
String datoGPS;
float dato_aceleracion = 9.8;
int estado_led = LOW;
int i = 0;
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pulsador, INPUT_PULLUP);
  Serial.begin(9600);   // Serial pc arduino
  miBT.begin(38400);		// comunicacion serie entre Arduino y el modulo a 38400 bps
  Serial1.begin(9600);  // GPS

  //Serial.println("Adafruit MMA8451 test!");

  if (! mma.begin()) {
    //Serial.println("Couldnt start");
    while (1);
  }
  //Serial.println("MMA8451 found!");
  mma.setRange(MMA8451_RANGE_2_G);
  //Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  //Serial.println("G");
}

void loop() {  
  datoGPS = GPS();
  dato_aceleracion = aceleracion();
  currentMillis = millis();

  if(dato_aceleracion < 3){
    kondizioa1 = true;
    Serial.println("kondizioa1");
  }
  if(kondizioa1 == true && dato_aceleracion > 22){
    previousMillisK1 = currentMillis;
    kondizioa2 = true;
    Serial.println("kondizioa2");
  }
  if(kondizioa2 == true && dato_aceleracion < 12 && dato_aceleracion > 8){
    previousMillisK1 = currentMillis;
    kondizioa3 = true;
    Serial.println("kondizioa3");
  }
  if(kondizioa3 == true){
    previousMillisK1 = currentMillis;
    estado = 2;
    kondizioa1 = false;
    kondizioa2 = false;
    kondizioa3 = false;
  }
  if(currentMillis - previousMillisK1 >= 2000){
    kondizioa1 = false;
    kondizioa2 = false;
    kondizioa3 = false;
    previousMillisK1 = currentMillis;
    Serial.println("BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB");
  }

  switch (estado){
  case 1:
    //Serial.println(i);
    i = 0;
    digitalWrite(ledPin, LOW);
    //Serial.println("Caso 1");
    estado_led = LOW;
    noTone(6); 
    datoGPS = GPS();
    dato_aceleracion = aceleracion();
    currentMillis = millis();
    
    if(dato_aceleracion < 3){
    kondizioa1 = true;
    Serial.println("kondizioa1");
  }
  if(kondizioa1 == true && dato_aceleracion > 22){
    previousMillisK2 = currentMillis;
    kondizioa2 = true;
    Serial.println("kondizioa2");
  }
  if(kondizioa2 == true && dato_aceleracion < 12 && dato_aceleracion > 8){
    previousMillisK2 = currentMillis;
    kondizioa3 = true;
    Serial.println("kondizioa3");
  }
  if(kondizioa3 == true){
    previousMillisK2 = currentMillis;
    estado = 2;
    kondizioa1 = false;
    kondizioa2 = false;
    kondizioa3 = false;
  }
  if(currentMillis - previousMillisK2 >= 2000){
    kondizioa1 = false;
    kondizioa2 = false;
    kondizioa3 = false;
    previousMillisK2 = currentMillis;
    Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
  }

  break;

  case 2:
  //Serial.println(i);
  GPS();
  // aceleracion();
  //Serial.println("Caso 2");
  //    digitalWrite(ledPin, estado_led);

    if (currentMillis - previousMillis >= 1000) { // save the last time you blinked the BUZZER
    
    // if the BUZZER is off turn it on and vice-versa:
    if (BUZZERSTATE == LOW) {
      BUZZERSTATE = HIGH;
      digitalWrite(ledPin, HIGH);
      estado_led = !estado_led;
      tone(6, 4000);
        i++;
      } 
      
      else {
      BUZZERSTATE = LOW;
      noTone(6);
      digitalWrite(ledPin, LOW);
    }
    previousMillis = currentMillis;
    }

    if (i == 10){
      if(contador == 0){
        miBT.println(datoGPS);
        //Serial.println(datoGPS);
        //Serial.println("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
        contador++;
      }
    }

    if(miBT.available() > 0 || digitalRead(8) == LOW){
    estado = 1;
    i = 0;
    contador = 0;
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
  Serial.print("z:");
  Serial.print(z);
  Serial.print(",");
  Serial.print("a:");
  Serial.print(22);
  Serial.print(",");
  Serial.print("b:");
  Serial.print(8);
  Serial.print(",");
  Serial.print("c:");
  Serial.print(12);
  Serial.print(",");
  Serial.print("3:");
  Serial.print(3);
  Serial.print(",");
  Serial.print("x:");
  Serial.print(x);
  Serial.print(",");
  Serial.print("Mod:");
  Serial.println(mod);
  return mod;
}