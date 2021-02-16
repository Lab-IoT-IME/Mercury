#include <Arduino.h>
#include "WiFi.h"
#include "WNM.h"
#include <iostream>
#include <string>

#define LED_BUILTIN 2 

//Global variables
String wifiConnected = "";
WNM::Wifi wifi;
std::vector<String> x;

//Functions declaration
String WifiScanOpenNet();
void WifiConnect(String ssid);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  x = wifi.getWifiList();  

  Serial.println("Setup");
}

void loop() {
  if(Serial.available()){
    String y = Serial.readStringUntil('\n');
    Serial.println("String capturada: "+y);
    Serial.println(x[std::atoi(y.c_str())]);
  }

  //delay(200000);

  /*
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

/*--- VERIFICA REDES WIFI ABERTAS ---*/
String WifiScanOpenNet() {
  Serial.println("Procurando redes WiFi...");

  int countWifi = WiFi.scanNetworks(); //Retorna a quantidade de redes wifi disponíveis
  int openWifi = 0; //Variável para contar quantas redes abertas foram encontradas
  String ssid = "";

  //Exibe as redes wifi encontradas
  if (countWifi > 0) {
    Serial.println(String(countWifi) + " redes encontradas.\n");
    delay(500);
    for (int i = 0; i < countWifi; ++i) {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(") ");
      if (WiFi.encryptionType(i) != WIFI_AUTH_OPEN) {
        Serial.println("*");
      } else {
        Serial.println();
        openWifi++;
      }
    }
  } else {
    Serial.println("Nenhuma rede encontrada.");
    return ssid;
  }
  delay(2000);

  //Exibe os dados da rede wifi aberta com melhor sinal
  if (openWifi > 0) {
    for (int i = 0; i < countWifi; ++i) {
      if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
        Serial.println("\nRede aberta com melhor sinal:");
        Serial.print("SSID: ");
        Serial.print(WiFi.SSID(i));
        Serial.print("\t\tSINAL: ");
        Serial.print(WiFi.RSSI(i));
        Serial.print("\t\tCANAL: ");
        Serial.println(WiFi.channel(i));
        Serial.println();
        ssid = WiFi.SSID(i);
        break; //Finaliza o loop
      }
    }
  } else {
    Serial.println("\nNenhuma rede aberta encontrada.\n");
  }
  delay(2000);

  return ssid;
}

/*--- EFETUA CONEXÃO COM REDE WIFI ---*/
void WifiConnect(String ssid) {
  char wifiSSID[50];
  ssid.toCharArray(wifiSSID, 50);

  WiFi.disconnect();    //Fecha conexão aberta
  WiFi.begin(wifiSSID); //Efetua a conexão com a rede wifi

  Serial.print("Conectando no WiFi " + ssid + ".");
  int times = 0;

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    times++;
    if (times == 250) break; //Mantém aguardando conexão por 15 segundos
    delay(100);
  }

  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = ssid;
    Serial.println("\nWiFi conectado!\n");
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    wifiConnected = "";
    Serial.println("\nNao foi possivel conectar.\n");
    digitalWrite(LED_BUILTIN, LOW);
  }
}