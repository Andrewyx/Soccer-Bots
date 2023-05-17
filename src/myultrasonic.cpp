#include <Arduino.h>
#include "myultrasonic.h"

const int leftTrigPin = 14;
const int leftEchoPin = 27;
const int rightTrigPin = 25;
const int rightEchoPin = 33;
float leftDistance;
float rightDistance;
float cleanedLD, cleanedRD;

unsigned long previousMillis = 0;
int interval = 1;
int interval2 = 200;
int leftTrigState = LOW;
int rightTrigState = LOW; //state of trigPin
unsigned long currentMillis;
int leftDuration, rightDuration; 

double leftKalman(double U){
  static const double F = 40;
  static const double D = 1.00;
  static double S = 10;
  static double L = 0;
  static double V_hat = 0;
  static double bozo = 0;
  bozo = L*D/(D*L*D+F);
  V_hat += + bozo*(U-D*V_hat);
  L = (1-bozo*D)*L+S;
  return V_hat;
}
double rightKalman(double U){
  static const double R = 40;
  static const double H = 1.00;
  static double Q = 10;
  static double P = 0;
  static double U_hat = 0;
  static double K = 0;
  K = P*H/(H*P*H+R);
  U_hat += + K*(U-H*U_hat);
  P = (1-K*H)*P+Q;
  return U_hat;
}

double midLineKalman(double U){
  static const double forty = 40;
  static const double floaty = 1.00;
  static double teny = 10;
  static double zero = 0;
  static double zerozero = 0;
  static double dangan = 0;
  dangan = zero*floaty/(floaty*zero*floaty+forty);
  zerozero += + dangan*(U-floaty*zerozero);
  zero = (1-dangan*floaty)*zero+teny;
  return zerozero;
}


void initUltrasonic() {
  pinMode(leftTrigPin, OUTPUT); 
  pinMode(leftEchoPin, INPUT); 
  pinMode(rightTrigPin, OUTPUT); 
  pinMode(rightEchoPin, INPUT); 
}

double midPointVal(double val1, double val2){
	return (val1 + val2)/2;
}

void meanDistance(float dRight, float dLeft){
	midLineKalman(midPointVal(-dRight, dLeft));
}

void runUltrasonic() {

		digitalWrite(leftTrigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(leftTrigPin, LOW);
		delayMicroseconds(2);
		leftDuration = pulseIn(leftEchoPin,HIGH);
		leftDistance = (leftDuration/2) / 29.1;
		Serial.print("Left distance:");
    cleanedLD = leftKalman(leftDistance);
    Serial.println(cleanedLD);

		digitalWrite(rightTrigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(rightTrigPin, LOW);	
		delayMicroseconds(2);
		rightDuration = pulseIn(rightEchoPin,HIGH);
		rightDistance = (rightDuration/2) / 29.1;	
		Serial.print("Right distance:");
    cleanedRD = rightKalman(rightDistance);
		Serial.println(cleanedRD);	
    
}