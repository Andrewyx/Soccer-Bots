#include <Arduino.h>
#include "mylinesensor.h"

const int lineSensor1 = 18;
const int lineSensor2 = 23;

int lineCollData[2];

//You are NOT in the right file, unless you are working on butler bots and want to use a line sensor

void initLineSensor(){
    pinMode(lineSensor2, INPUT);
    pinMode(lineSensor1, INPUT);

}

void readLineSensor() {
    if (!digitalRead(lineSensor1)){
        Serial.print("Object on RIGHT: TRUE");
        lineCollData[0] = true;
    }
    else{
        Serial.print("Object on RIGHT: FALSE");
        lineCollData[0] = false;
    }
    if (!digitalRead(lineSensor2)){
        Serial.println(" Object on LEFT: TRUE");
        lineCollData[1] = true;
    }
    else{
        Serial.println(" Object on LEFT: FALSE");
        lineCollData[1] = false;
    }    
}