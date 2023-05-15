#include <Arduino.h>
#include "myultrasonic.h"

const int leftTrigPin = 14;
const int leftEchoPin = 27;
const int rightTrigPin = 25;
const int rightEchoPin = 33;
float leftDistance;
float rightDistance;
unsigned long previousMillis = 0;
int interval = 1;
int interval2 = 200;
int leftTrigState = LOW;
int rightTrigState = LOW; //state of trigPin
unsigned long currentMillis;
int leftDuration, rightDuration; 

void initUltrasonic() {
  pinMode(leftTrigPin, OUTPUT); 
  pinMode(leftEchoPin, INPUT); 
  pinMode(rightTrigPin, OUTPUT); 
  pinMode(rightEchoPin, INPUT); 
}

void runUltrasonic() {

		digitalWrite(leftTrigPin, HIGH);

		delayMicroseconds(10);
		digitalWrite(leftTrigPin, LOW);
		delayMicroseconds(2);
		leftDuration = pulseIn(leftEchoPin,HIGH);
		leftDistance = (leftDuration/2) / 29.1;
		Serial.print("Left distance:");
		Serial.println(leftDistance);

		digitalWrite(rightTrigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(rightTrigPin, LOW);	
		delayMicroseconds(2);
		rightDuration = pulseIn(rightEchoPin,HIGH);
		rightDistance = (rightDuration/2) / 29.1;	
		Serial.print("Right distance:");
		Serial.println(rightDistance);	
}