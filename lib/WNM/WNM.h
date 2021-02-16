#include <Arduino.h>
#include "WiFi.h"
#include <vector>

namespace WNM {
    class Wifi{
        private:
            std::vector<String> wifiList;
        public:
            std::vector<String> getWifiList();
    };
};