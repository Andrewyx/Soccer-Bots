#include <Arduino.h>
#include "mylinesensor.h"

const int onBoardLED = 2;
bool isBlocked = false;

void initLineSensor(){
    pinMode(lineSensor1, INPUT);
    pinMode(onBoardLED, OUTPUT);

}

void readLineSensor() {
    isBlocked = digitalRead(onBoardLED);
    if (isBlocked){
        digitalWrite(onBoardLED, HIGH);
    }
    else{
        digitalWrite(onBoardLED, LOW);
    }
}