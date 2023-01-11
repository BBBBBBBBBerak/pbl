#include <SoftwareSerial.h>	// libreria que permite establecer pines digitales para comunicacion serie

SoftwareSerial miBT(10, 11); 	// pin 10 como RX, pin 11 como TX
int i = 0;

void setup(){
  Serial.begin(9600);		// comunicacion de monitor serial a 9600 bps
  Serial.println("Listo");	// escribe Listo en el monitor
  miBT.begin(38400);		// comunicacion serie entre Arduino y el modulo a 38400 bps
}

void loop(){
 miBT.write("Bengoa");
}