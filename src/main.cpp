#include <Arduino.h>
#include "WNM.h"
#include <string>
#include <HTTPClient.h>

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#include "Shared.h"

#define REPORTING_PERIOD_MS 1000
#define DATA_SIZE 10

#include "Button.h"
Button prev(34);
Button next(35);
Button back(32);
Button slct(33);

#include "Menu.h"
#include "API.h"

//Global variables
String wifiConnected = "";
WNM::Wifi wifi(NULL);
char SSID[] = "DaviPontes";
char PASS[] = "ricardofranco";

PulseOximeter pox;
float BPM, SpO2;

String baseUrl = "http://hermes-iot.azurewebsites.net";
API api(baseUrl);

uint32_t tsLastReport = 0;

//Functions declaration
unsigned long lastBeat;
void onBeatDetected()
{
  Serial.println("Beat!");
  lastBeat = millis();
}
void stop();
void start();
void cancel();
Display display(19, 23, 18, 17, 16, 15);
Menu init_menu(&display, "MERCURY", dp_close, dp_menu, cancel, start);
Menu* current_menu;

void updateDisplay(){
  String txt = "HR:" + String(BPM, 0) + " | SpO2:" + String(SpO2, 0);
  display.draw_body_fast(txt.c_str());
}

void clearDisplay(){
  display.draw_body_fast("----------------");
}

bool getData = false;
int nData = 0;
float hrData[DATA_SIZE], spo2Data[DATA_SIZE];

void start(){
  pox.resume();
  getData = true;
  nData = 0;
  display.draw_status(dp_heart, NULL);
}

void stop(){
  pox.shutdown();
  getData = false;
  display.draw_status(NULL, NULL);
}

void cancel(){
  stop();
  clearDisplay();
}

int lastSetId = 10;
void sendData(){
  display.draw_status(NULL, dp_wifi);
  api.sendData(0, 0, hrData);
  api.sendData(0, 1, hrData);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  prev.begin();
  next.begin();
  back.begin();
  slct.begin();

  display.begin();
  
  current_menu = &init_menu;
  current_menu->show();

  wifi.connect(SSID, PASS);
  wifi.printInfo();

  api.begin("davihugomp@gmail.com", "123456");

  if (!pox.begin()) {
    Serial.println("POX Failed");
  } else {
    Serial.println("POX Success");
    pox.setOnBeatDetectedCallback(onBeatDetected); 
  };

  pox.setIRLedCurrent(MAX30100_LED_CURR_50MA);
  pox.shutdown();
} 

void loop() {
  // Make sure to call update as fast as possible
  pox.update();

  // Asynchronously dump heart rate and oxidation levels to the serial
  // For both, a value of 0 means "invalid"
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS && true) {
    Serial.print("Heart rate:");
    Serial.print(BPM);
    Serial.print("bpm / SpO2:");
    Serial.print(SpO2);
    Serial.println("%");
    tsLastReport = millis();
    if(getData){
      Serial.println(nData);
      if(nData > 4 && nData < 15){
        hrData[nData-5] = BPM;
        spo2Data[nData-5] = SpO2;
      }
      if(nData == 15){
        pox.shutdown();
        sendData();
        stop();
      };
      nData++;
    }
  }

  if(prev.pressed()){
    current_menu = current_menu->nav_left();
  }
  if(next.pressed()){
    current_menu = current_menu->nav_right();
  }
  if(back.pressed()){
    current_menu = current_menu->opt_left();
  }
  if(slct.pressed()){
    current_menu = current_menu->opt_right();
  }
}