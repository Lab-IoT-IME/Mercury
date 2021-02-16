#include "WNM.h"

using namespace WNM;

std::vector<String> Wifi::getWifiList(){
    int countWifi = WiFi.scanNetworks();
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
        }
        list.push_back(WiFi.SSID(i));
    }
    wifiList = list;
    return wifiList;
}