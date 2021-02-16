#include <Arduino.h>
#include "WNM.h"

//Global variables
String wifiConnected = "";
WNM::Wifi wifi;
char SSID[] = "Network_SSID";
char PASS[] = "Network_PASS";

//Functions declaration
String WifiScanOpenNet();
void WifiConnect(String ssid);

void setup() {
  Serial.begin(115200);
  delay(500);

  wifi.connect(SSID, PASS);
  wifi.printInfo();

  Serial.setTimeout(2000);
}

void loop() {
  /*
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