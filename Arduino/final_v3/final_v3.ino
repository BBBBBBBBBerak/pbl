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


const int ledPin = 7;
const int pulsador = 8;
int ledState = LOW;
unsigned long previousMillis = 0;
unsigned long previousMillisE = 0;
unsigned long previousMillisW = 0;
unsigned long previousMillisw = 0;
unsigned long previousMillisK1 = 0;
unsigned long previousMillisK2 = 0;
unsigned long K2 = 0;
unsigned long currentMillis = 0;
int estado = 1;
int BUZZERSTATE = 0;
int contador = 0;
int contadorE = 0;
int contadorW = 0;
int contadorw = 0;

bool kondizioa1 = false;
bool kondizioa2 = false;
bool kondizioa3 = false;
bool kondizioa4 = false;

Adafruit_MMA8451 mma = Adafruit_MMA8451();

SoftwareSerial miBT(10, 11); 	// pin 10 como RX, pin 11 como TX
SoftwareSerial Serial1(4, 3); //Asignar el puerto a los pines(Al GPS)

TinyGPS gps;
String dato_gps;
String datoGPS;
float dato_aceleracion = 9.8;
int estado_led = LOW;
int i = 0;
int j = 0;
int e = 0;
int w = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(pulsador, INPUT_PULLUP);
  Serial.begin(9600);   // Serial pc arduino
  miBT.begin(38400);		// comunicacion serie entre Arduino y el modulo a 38400 bps
  Serial1.begin(9600);  // GPS


  if (! mma.begin()) {
    while (1);
  }
  mma.setRange(MMA8451_RANGE_2_G);
  //Serial.print("Range = "); Serial.print(2 << mma.getRange());  

  estado = 1;
}

void loop() {  
  switch (estado){
    case 1:
      datoGPS = GPS();
      dato_aceleracion = aceleracion();
      currentMillis = millis();
      
      i = 0;
      digitalWrite(ledPin, LOW);
      estado_led = LOW;
      noTone(6); 
    
      if(dato_aceleracion < 6){
        kondizioa1 = true;
      }
      if(kondizioa1 == true && dato_aceleracion > 20){
        previousMillisK2 = currentMillis;
        kondizioa2 = true;
      }
      if(kondizioa2 == true && dato_aceleracion < 12 && dato_aceleracion > 8){
        previousMillisK2 = currentMillis;
        kondizioa3 = true;
      }
      if(kondizioa3 == true){
        if(contadorw == 0){
          previousMillisw = currentMillis;
          contadorw++;
        }
        if(currentMillis - previousMillisw >= 1500){
          if(contadorW == 0){
            previousMillisW = currentMillis;
            contadorW++;
          }
        if(currentMillis - previousMillisW >= 100){
          if(dato_aceleracion < 10.8 && dato_aceleracion > 9.2){
            w++;
          }
        else{
          w = 0;
          kondizioa1 = false;
          kondizioa2 = false;
          kondizioa3 = false;
          kondizioa4 = false;
        }
      }
      if(w >= 50){
        previousMillisK1 = currentMillis;
        kondizioa4 = true;
      }
    }
  }
      if(kondizioa4 == true){
        previousMillisK2 = currentMillis;
        estado = 2;
        kondizioa1 = false;
        kondizioa2 = false;
        kondizioa3 = false;
        kondizioa4 = false;
      }
      if(currentMillis - previousMillisK2 >= 6000){
        kondizioa1 = false;
        kondizioa2 = false;
        kondizioa3 = false;
        kondizioa4 = false;
        previousMillisK2 = currentMillis;
      }

      if(currentMillis - previousMillisE >= 200 && digitalRead(8) == LOW){
        if(contadorE == 0){
          previousMillisE = currentMillis;
          contadorE++;
        }
        else{
          e++;
        }
        if(e >= 100){
          estado = 3;
        }
      }
      else{
        e = 0;
      }
    break;

    case 2:
      datoGPS = GPS();
      dato_aceleracion = aceleracion();
      currentMillis = millis();
      
      if (currentMillis - previousMillis >= 1000) {
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
          contador++;
          Serial.println("EMERGENCIA");
        }
      }

      if(miBT.available() > 0 || digitalRead(8) == LOW){
        estado = 1;
        i = 0;
        contador = 0;
        w = 0;
        contadorw = 0;
        contadorW = 0;
      }
    break;

    case 3:
      datoGPS = GPS();
      dato_aceleracion = aceleracion();
      currentMillis = millis();

      if(j <= 75){
        if (currentMillis - previousMillis >= 50) {
          if (BUZZERSTATE == LOW) {
            BUZZERSTATE = HIGH;
            digitalWrite(ledPin, HIGH);
            estado_led = !estado_led;
            tone(6, 4000);
            j++;
          } 
          else {
            BUZZERSTATE = LOW;
            noTone(6);
            digitalWrite(ledPin, LOW);
            j++;
          }
          previousMillis = currentMillis;
        }

      }
      else{
        miBT.println(datoGPS);
        estado = 1;
        Serial.println(" EMERGENCIA ");
        contadorE = 0;
        j = 0;
        e = 0;
      }
    break;
  }
}

String GPS(){ 
  float flat, flon;
  unsigned long age;
  gps.f_get_position(&flat, &flon, &age);

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
  Serial.print("20:");
  Serial.print(20);
  Serial.print(",");
  Serial.print("10.8:");
  Serial.print(10.8);
  Serial.print(",");
  Serial.print("9.2:");
  Serial.print(9.2);
  Serial.print(",");
  Serial.print("3:");
  Serial.print(6);
  Serial.print(",");
  Serial.print("Mod:");
  Serial.println(mod);
  return mod;
}