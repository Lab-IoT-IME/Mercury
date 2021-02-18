#include <Arduino.h>
#include "WNM.h"

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#include "Shared.h"

#define LED_RED 2
#define LED_GREEN 4
#define REPORTING_PERIOD_MS 1000

//Global variables
String wifiConnected = "";
WNM::Wifi wifi(LED_GREEN);
char SSID[] = "JKLMN 2.4";
char PASS[] = "pandemia2020";
PulseOximeter pox;
float BPM, SpO2;

uint32_t tsLastReport = 0;

//Functions declaration
String WifiScanOpenNet();

void onBeatDetected()
{
  Serial.println("Beat!");
  blink(LED_RED, 200);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
  delay(500);

  wifi.connect(SSID, PASS);
  wifi.printInfo();

  //Serial.setTimeout(2000);

  if (!pox.begin()) {
    Serial.println("FAILED");
    for(;;);
  } else {
    Serial.println("SUCCESS");
    pox.setOnBeatDetectedCallback(onBeatDetected);
    
  };

  //pox.setIRLedCurrent(MAX30100_LED_CURR_14_2MA);
}



void loop() {
  // Make sure to call update as fast as possible
  pox.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(BPM);
    Serial.print("bpm / SpO2:");
    Serial.print(SpO2);
    Serial.println("%");
    tsLastReport = millis();
  }

  /*
  digitalWrite(LED, !digitalRead(LED));
  Serial.println("LED");
  delay(1000);
  if(Serial.available()){
    String y = Serial.readStringUntil('\n');
    Serial.println("String capturada: "+y);
    Serial.println(x[std::atoi(y.c_str())]);
  }

  //Retorna o nome da rede aberta com melhor sinal
  String ssid = WifiScanOpenNet();

  if (ssid != "" && ssid != wifiConnected) {
    //Se retornou nome de rede wifi e a rede for diferente da atual, tenta a conexão
    //WifiConnect(ssid);
    ;

  } else if (ssid != "" && ssid == wifiConnected) {
    //Se retornou nome de rede wifi e a rede for igual a atual, permanece conectado
    Serial.println("Ja conectado na rede!\n");

  } else if (ssid == "" || WiFi.status() != WL_CONNECTED) {
    //Se não retornou nome de rede wifi ou o status da conexão estiver diferente de conectado
    WiFi.disconnect();
    wifiConnected = "";
    digitalWrite(LED_BUILTIN, LOW);
  }

  delay(20000);
  */
}