#include <Arduino.h>
#include "WiFi.h"
#include <vector>
#include <iostream>
#include "Shared.h"

namespace WNM
{
    class Wifi {
        private:
            int LED; 
            int openWifi;
            int countWifi;
            String wifiConnected;
            std::vector<String> wifiList;
            bool connectCheck(const char *SSID);
        public:
            Wifi(int statusLED):LED(statusLED){};
            WiFiClass WiFi;
            std::vector<String> getList();
            bool connect();
            bool connect(const char *SSID);
            bool connect(const char *SSID, const char *Password);
            void printInfo();
    };
};