#include <Arduino.h>
#include "myultrasonic.h"

const int leftTrigPin = 14;
const int leftEchoPin = 27;
const int rightTrigPin = 12;
const int rightEchoPin = 13;
float leftDistance;
float rightDistance;
unsigned long previousMillis = 0;
int interval = 1;
int interval2 = 200;
int leftTrigState = LOW;
int rightTrigState = LOW; //state of trigPin
int printState = LOW;
unsigned long currentMillis;

void initUltrasonic() {
  pinMode(leftTrigPin, OUTPUT); 
  pinMode(leftEchoPin, INPUT); 
  pinMode(rightTrigPin, OUTPUT); 
  pinMode(rightEchoPin, INPUT); 
}

void runUltrasonic() {
	currentMillis = millis(); //time in milliseconds from which the code was started
	if (currentMillis-previousMillis >= interval) { //check "blink without delay" code
		previousMillis = currentMillis;
		leftTrigState = !leftTrigState;
		rightTrigState = !rightTrigState;
	}
	// printing if statement
	if (currentMillis-previousMillis >= interval2) { //check "blink without delay" code
		previousMillis = currentMillis;
		if (printState == LOW){
			(printState = HIGH);
		}
		else {
			(printState = LOW);
		}
	}
	if(leftTrigState == HIGH){
		digitalWrite(leftTrigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(leftTrigPin, LOW);
		leftTrigState = LOW;
	}

	if(rightTrigState == HIGH){
		digitalWrite(rightTrigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(rightTrigPin, LOW);
		rightTrigState = LOW;
	}
	int leftDuration,rightDuration; 
	leftDuration = pulseIn(leftEchoPin,HIGH);
	rightDuration = pulseIn(rightEchoPin,HIGH);
	leftDistance = (leftDuration/2) / 29.1;
	rightDistance = (rightDuration/2) / 29.1;
if (printState = HIGH){
    Serial.print("Left distance:");
  	Serial.println(leftDistance);
    Serial.print("Right distance:");
  	Serial.println(rightDistance);
}

//left ~130
//right ~100

}