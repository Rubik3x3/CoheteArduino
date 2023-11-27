/*
--------------------------------------------
-------- COHETE DE AGUA CON ARDUINO --------
--------------------------------------------
------- Franco Salvador Talarico 3°1 -------
--------------------------------------------
*/

#include <Wire.h>		// Librería para Bus I2C
#include <Adafruit_Sensor.h>	// Librerías para el sensor BMP280
#include <Adafruit_BMP280.h>
#include <Servo.h>

Servo servo;
Adafruit_BMP280 bmp;

float ALTURA, ALTURA_MAX;
float TEMPERATURA;		
float PRESION, P0;

bool PARACAIDAS = false;
const int pinBuzzer = 9;
const int pinServo = 8;

// NOTAS DE INICIO
const int notas[] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494};
const int cantidadNotas = 10;

void setup() {
  Serial.begin(9600);

  Serial.println("INICIANDO CÓDIGO.");

  if ( !bmp.begin() ) {	// SI FALLA LA COMUNICACIÓN CON EL SENSOR
    Serial.println("No se encontró el sensor BMP280.");	
    error();
  }
  servo.attach(pinServo);
  servo.write(0); // MOVER EL SERVO A LA POSICIÓN INICIAL

  P0 = bmp.readPressure()/100;
  ALTURA = bmp.readAltitude(P0);
  ALTURA_MAX = ALTURA;

  Serial.println("SERVO y SENSOR LISTOS");

  //NOTAS DE INICIO DEL PROGRAMA
  for (int iNota = 0; iNota < cantidadNotas; iNota++)
  {
   tone(pinBuzzer, notas[iNota]);
   delay(100);
  }
  noTone(pinBuzzer);
}

void error(){
  while (1){
    tone(pinBuzzer, 1500);
    delay(250);
    noTone(pinBuzzer);
  }
}

void mostrarDatos(int altura, int temperatura, int presion) {
  Serial.println("-------------------------------");
  Serial.print("Temperatura: ");
  Serial.print(TEMPERATURA); // Temperatura en °C
  Serial.print(" °C ");
    
  Serial.print("Presion: ");
  Serial.print(PRESION/100); //Presion en hPa 
  Serial.println(" hPa");

  Serial.print("Altura Aprox: ");
  Serial.print(bmp.readAltitude(P0));	//Altura
  Serial.println(" m");		
}

void loop() {
  ALTURA = bmp.readAltitude(P0);
  TEMPERATURA = bmp.readTemperature();
  PRESION = bmp.readPressure();	

  mostrarDatos(ALTURA,TEMPERATURA,PRESION);

  //Calcular la altura máxima
  if (ALTURA > ALTURA_MAX){
    ALTURA_MAX = ALTURA;
  }

  // Si detecta que está bajando y el paracaídas no se abrió.
  if (ALTURA_MAX > ALTURA+1.5 && PARACAIDAS == false){
    PARACAIDAS = true;
    servo.write(90);
    Serial.println("SE ABRIO EL PARACAIDAS");
    tone(pinBuzzer, 1046);
    delay(1000);
    noTone(pinBuzzer);
  }

  delay(25);
}