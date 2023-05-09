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
int interval2 = 1000;
int leftTrigState = LOW;
int rightTrigState = LOW; //state of trigPin
int printState = LOW;

void initUltrasonic() {
  pinMode(leftTrigPin, OUTPUT); 
  pinMode(leftEchoPin, INPUT); 
  pinMode(rightTrigPin, OUTPUT); 
  pinMode(rightEchoPin, INPUT); 
}

void runUltrasonic() {
	unsigned long currentMillis = millis(); //time in milliseconds from which the code was started
	if (currentMillis-previousMillis >= interval) { //check "blink without delay" code
		previousMillis = currentMillis;
		if (leftTrigState == LOW){
			(leftTrigState = HIGH);
		}
		else {
			(leftTrigState = LOW);
		}
		if (rightTrigState == LOW){
			(rightTrigState = HIGH);
		}
		else {
			(rightTrigState = LOW);
		}
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
	digitalWrite(leftTrigPin,leftTrigState);
	digitalWrite(rightTrigPin,rightTrigState);
	int leftDuration,rightDuration; 
	leftDuration = pulseIn(leftEchoPin,HIGH);
	rightDuration = pulseIn(rightEchoPin,HIGH);
  if (leftDuration > 1){
	leftDistance = (leftDuration/2) / 29.1;
  }
  if (rightDuration > 1){
	rightDistance = (rightDuration/2) / 29.1;
  }
	if (printState = HIGH){
    Serial.print("Left distance:");
  	Serial.println(leftDistance);
    Serial.print("Right distance:");
  	Serial.println(rightDistance);
}


}