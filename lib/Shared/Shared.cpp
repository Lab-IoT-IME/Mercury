#include "Shared.h"

void blink(int LED, int time){
    digitalWrite(LED, LOW);
    for(int i=0; i<2 ;i++){
        digitalWrite(LED,!digitalRead(LED));
        delay(time/2);
    }
}