#include "WNM.h"

using namespace WNM;

std::vector<String> Wifi::getList(){
    Serial.println("Getting wifiList");
    openWifi = 0;
    countWifi = WiFi.scanNetworks();
    std::vector<String> list = {};
    for(int i=0; i<countWifi; i++){
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
        list.push_back(WiFi.SSID(i));
    }
    wifiList = list;
    return wifiList;
}

bool Wifi::connect(){
    String ssid;

    if(wifiList.size()==0){
        Serial.println("\nThe wifiList is empty!");
        this->getList();
    }

    if (openWifi > 0) {
        for (int i = 0; i < countWifi; ++i) {
            if (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) {
                Serial.println("\nBest open wifi available:");
                Serial.print("SSID: ");
                Serial.print(WiFi.SSID(i));
                Serial.print("\t\tSIGNAL: ");
                Serial.print(WiFi.RSSI(i));
                Serial.print("\t\tCHANNEL: ");
                Serial.println(WiFi.channel(i));
                ssid = WiFi.SSID(i);
                break;
            }
        }
    } else {
        Serial.println("There is no open Wifi.");
        return false;
    }

    return this->connect(ssid.c_str());
}

bool Wifi::connect(const char *SSID){
    WiFi.disconnect();
    WiFi.begin(SSID);
    return this->connectCheck(SSID);
}

bool Wifi::connect(const char *SSID, const char *Password){
    WiFi.disconnect();
    WiFi.begin(SSID, Password);
    return this->connectCheck(SSID);
}

bool Wifi::connectCheck(const char *SSID){
    Serial.print("Connecting to WiFi ");
    Serial.print(SSID);
    Serial.print('.');

    int times = 0;

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        times++;
        if (times == 250) break;
        blink(LED,200);
    }

    if (WiFi.status() == WL_CONNECTED) {
        wifiConnected = SSID;
        Serial.println("\nWiFi connected!\n");
        return true;
    } else {
        wifiConnected = "";
        Serial.println("\nCouldn't connect.\n");
        return false;
    }
}

void Wifi::printInfo(){
    if(wifiConnected == ""){
        Serial.println("WiFi not connected!");
    }else{
        Serial.println("WiFi INFORMATIONS:");
        Serial.println("- SSID: "+WiFi.SSID());
        Serial.println("- SIGNAL: "+String(WiFi.RSSI()));
        Serial.println("- CHANNEL: "+String(WiFi.channel()));
        Serial.println("- IP: "+WiFi.localIP().toString());
        Serial.println("- MAC: "+WiFi.macAddress());
    }
}