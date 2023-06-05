#include <Arduino.h>
#include "myultrasonic.h"

//THIS FILE IS MEANT FOR BUTLER BOTS, NOT SOCCER BOTS

//match this value with regListLen
#define N 10

//This is meant for tri HC-SR04 Ultrasonic Sensor Array to autonomously navigate
const int leftTrigPin = 23;
const int leftEchoPin = 18;
const int rightTrigPin = 25;
const int rightEchoPin = 33;
const int frontTrigPin = 32;
const int frontEchoPin = 21;
float leftDistance, rightDistance, frontDistance;
float cleanedLD, cleanedRD, cleanedFD;

//Linear Regression Vars and Vals to clean up data
float x[N], y[N], sum_x = 0, sum_x2 = 0, sum_y = 0, sum_xy = 0, a, b;
static unsigned int linearRegIndex = 0; 
static int regListLen = 10;
float deviationSlope = 0;

unsigned long previousMillis = 0;
unsigned long currentMillis;
int leftDuration, rightDuration, frontDuration; 

//filters to clean sensor readings
#pragma region KalmanFilters

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

double frontKalman(double U){
  static const double gooba = 40;
  static const double ligma = 1.00;
  static double sugma = 10;
  static double dooma = 0;
  static double sigma = 0;
  static double kenya = 0;
  kenya = dooma*ligma/(ligma*dooma*ligma+gooba);
  sigma += + kenya*(U-ligma*sigma);
  dooma = (1-kenya*ligma)*dooma+sugma;
  return sigma;
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

#pragma endregion KalmanFilters

#pragma region LinearRegressionMethods

float midPointVal(float val1, float val2){
	return (val1 + val2)/2;
}

float meanDistance(float dRight, float dLeft){
	return midLineKalman(midPointVal(-dRight, dLeft));
}

float linearReg(float inputVal){
  
  //change this to alter amount of sample points
  x[linearRegIndex] = linearRegIndex;
  y[linearRegIndex] = inputVal;
  /* Calculating Required Sum */
 
  /* Calculating a and b */

  if(linearRegIndex < regListLen){
    linearRegIndex += 1;
  }
  else{
    for(int i = 0;i < regListLen;i++){
        sum_x = sum_x + x[i];
        sum_x2 = sum_x2 + x[i]*x[i];
        sum_y = sum_y + y[i];
        sum_xy = sum_xy + x[i]*y[i];
    }
    b = (regListLen * sum_xy - sum_x * sum_y) / (regListLen * sum_x2 - sum_x * sum_x);    
    linearRegIndex = 0;
    
  }
  Serial.print(" Slope Val: ");
  Serial.println(b); 
  return b;
  //this one is useless
  //a = (sum_y - b * sum_x) / n;
}

#pragma endregion LinearRegressionMethods

#pragma region Functionality

void initUltrasonic() {
  pinMode(leftTrigPin, OUTPUT); 
  pinMode(leftEchoPin, INPUT); 
  pinMode(rightTrigPin, OUTPUT); 
  pinMode(rightEchoPin, INPUT); 
  pinMode(frontTrigPin, OUTPUT); 
  pinMode(frontEchoPin, INPUT); 
}

void runUltrasonic() {

		digitalWrite(leftTrigPin, LOW);
		delayMicroseconds(2);
		digitalWrite(leftTrigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(leftTrigPin, LOW);
		leftDuration = pulseIn(leftEchoPin,HIGH);
		leftDistance = (leftDuration/2) / 29.1;
		Serial.print("Left distance:");
    cleanedLD = leftKalman(leftDistance);
    Serial.print(cleanedLD);

		digitalWrite(rightTrigPin, LOW);	
		delayMicroseconds(2);
		digitalWrite(rightTrigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(rightTrigPin, LOW);	
		rightDuration = pulseIn(rightEchoPin,HIGH);
		rightDistance = (rightDuration/2) / 29.1;	
		Serial.print(" Right distance:");
    cleanedRD = rightKalman(rightDistance);
		Serial.print(cleanedRD);	

    digitalWrite(frontTrigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(frontTrigPin, LOW);	
		delayMicroseconds(2);
		frontDuration = pulseIn(frontEchoPin,HIGH);
		frontDistance = (frontDuration/2) / 29.1;	
		Serial.print("Front distance:");
    cleanedFD = frontKalman(frontDistance);
		Serial.println(cleanedFD);	

    deviationSlope = linearReg(meanDistance(cleanedRD, cleanedLD));
    
}

#pragma endregion Functionality