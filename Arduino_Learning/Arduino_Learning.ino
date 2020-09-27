//Bluetooth Module
#include <SoftwareSerial.h>
SoftwareSerial BLE(5,4);
char command;

//Temp&Hum Module
#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 13
DHT dht(DHTPIN, DHTTYPE);

//Led Config
#define ledPin 2

//MAX30100 Module
#include "MAX30100.h"
#include <Wire.h>
MAX30100 pox;

//Ohm Meter
int analogPin = 0;
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 10000;
float R2 = 0;
float buffer = 0;

unsigned long lastWarning = 0;

void setup() {
  Serial.begin(9600);
  BLE.begin(9600);
  //Wire.begin();
  
  pinMode(ledPin, OUTPUT);
  
  delay(1000);

  //pox.begin(pw1600, i50, sr100);
  
  dht.begin();
  Serial.println("Let's Go!");
}

void loop() {
  if(BLE.available()){
    command = BLE.read();
    Serial.println(command);
    if(!menu(command) && (millis() - lastWarning) > 500){
      lastWarning = millis();
      Serial.println("Invalid Bluetooth Command!");
    }
  }

  if(Serial.available()){
    command = Serial.read();
    if(!menu(command) && (millis() - lastWarning) > 500){
      lastWarning = millis();
      Serial.println("Invalid Serial Command!");
    }
  }

  //pox.readSensor();
  //Serial.println(meanDiff(pox.IR));
}

int menu(char cmd) {
  if(cmd == 'T'){
    readTempHum(); 
    lastWarning = millis();
    return 1; 
  }else if(cmd == 'L'){
    light();
    lastWarning = millis();
    return 1;
  }else if(cmd == 'O'){
    ohm();
    lastWarning = millis();
    return 1;
  }else{
    return 0;
  }
}

void readTempHum() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  Serial.print("Umidade = ");
  Serial.print(h);
  Serial.print(" %  ");
  Serial.print("Temperatura = ");
  Serial.print(t);
  Serial.println(" Celsius ");
}

void light() {
  if(digitalRead(ledPin)){
    digitalWrite(ledPin, LOW);
    Serial.println("Light Off");
  }else{
    digitalWrite(ledPin, HIGH);
    Serial.println("Light On");
  }
}  

long meanDiff(int M) {
  #define LM_SIZE 15
  static int LM[LM_SIZE];      // LastMeasurements
  static byte index = 0;
  static long sum = 0;
  static byte count = 0;
  long avg = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  avg = sum / count;
  return avg - M;
}

void ohm(){
  raw=analogRead(analogPin);
  if(raw){
    buffer=raw*Vin;
    Vout=buffer/1024.0;
    buffer=(Vin/Vout) - 1;
    R2=R1*buffer;
    Serial.print("Vout: ");
    Serial.print(Vout);
    Serial.print("  |  R2: ");
    Serial.println(R2);
  }else{
    Serial.println("Analog Error");
  }
}
