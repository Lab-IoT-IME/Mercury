#include <Arduino.h>
#include "WiFi.h"
#include <vector>
#include <iostream>

namespace WNM {
    class Wifi{
        private:
            int openWifi;
            int countWifi;
            String wifiConnected;
            std::vector<String> wifiList;
            bool connectCheck(const char *SSID);
        public:
            WiFiClass WiFi;
            std::vector<String> getList();
            bool connect();
            bool connect(const char *SSID);
            bool connect(const char *SSID, const char *Password);
            void printInfo();
    };
};