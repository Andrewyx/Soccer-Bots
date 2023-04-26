#include <Arduino.h>
#include <ESP32Servo.h>
#include <math.h>
#include "mymotor.h"

int leftMotor, rightMotor;
int isForward, isTurn;
Servo leftServo, rightServo;

int DegreeInRadian(double x){
  return x * (3.1415/180);;
}

void initServoLib(){
  // 16 servo objects can be created on the ESP32
  
  int pos = 0;    // variable to store the servo position
  // Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33   
  	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	leftServo.setPeriodHertz(50);
  rightServo.setPeriodHertz(50);
	leftServo.attach(MotorA1, 500, 2400);
  rightServo.attach(MotorB1, 500, 2400);

}

void runButlerMotor(){
  isForward = (int)(rawIntData[2] * sin(rawIntData[3]*PI/180));
  isTurn = (int)(rawIntData[2] * cos(rawIntData[3]*PI/180));
  
  Serial.print("Cos: ");
  Serial.print(cos(rawIntData[3]*PI/180));
  Serial.print(" Speed: ");
  Serial.print(rawIntData[2]);
  Serial.print(" Angle: ");
  Serial.print(rawIntData[3]);  
  Serial.print(" ForwardVal: ");
  Serial.print(isForward);
  Serial.print(" TurnVal: ");
  Serial.println(isTurn);
  
    
  if (rawIntData[2] == 0){
    leftServo.write(90);
    rightServo.write(90);
  }  
  else {
      leftMotor = abs(isForward) + abs(isTurn);
      rightMotor = abs(isForward);
      leftMotor = constrain(leftMotor, 0, 100);
      rightMotor = constrain(rightMotor, 0, 100);
      leftMotor = map(leftMotor, -100, 100, 0, 180);
      rightMotor = map(rightMotor, -100, 100, 0, 180);
      leftServo.write(leftMotor);
      rightServo.write(rightMotor);    
  }
}



void initL298N(){
  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);

  digitalWrite(MotorA1, LOW);
  digitalWrite(MotorA2, LOW);
  digitalWrite(MotorB1, LOW);
  digitalWrite(MotorB2, LOW);  
}

void calcMotor(){
  isForward = (int)(rawIntData[2] * sin(rawIntData[3]*PI/180));
  isTurn = (int)(rawIntData[2] * cos(rawIntData[3]*PI/180));
  
  Serial.print("Cos: ");
  Serial.print(cos(rawIntData[3]*PI/180));
  Serial.print(" Speed: ");
  Serial.print(rawIntData[2]);
  Serial.print(" Angle: ");
  Serial.print(rawIntData[3]);  
  Serial.print(" ForwardVal: ");
  Serial.print(isForward);
  Serial.print(" TurnVal: ");
  Serial.println(isTurn);
  
    
  if(rawIntData[2] == 0){
    A1PWM = 0;
    A2PWM = 0;
    B1PWM = 0;
    B2PWM = 0;
    isMoving = false;
  }

  else{
    isMoving = true;
  }

  if (isMoving){

    if(isForward < 0 && isTurn > 0){
      leftMotor = abs(isForward) + abs(isTurn);
      rightMotor = abs(isForward);
      leftMotor = constrain(leftMotor, 0, 100);
      rightMotor = constrain(rightMotor, 0, 100);
      A1PWM = map(leftMotor, 0, 100, 0, 255);
      A2PWM = 0;
      B1PWM = map(rightMotor, 0, 100, 0, 255);
      B2PWM = 0;      
    }
    else if(isForward < 0 && isTurn < 0){
      leftMotor = abs(isForward);
      rightMotor = abs(isForward) + abs(isTurn);
      leftMotor = constrain(leftMotor, 0, 100);
      rightMotor = constrain(rightMotor, 0, 100);
      A1PWM = map(leftMotor, 0, 100, 0, 255);
      A2PWM = 0;
      B1PWM = map(rightMotor, 0, 100, 0, 255);
      B2PWM = 0;        
    }
    else if(isForward > 0 && isTurn > 0){
      leftMotor = abs(isForward) + abs(isTurn);
      rightMotor = abs(isForward);
      leftMotor = constrain(leftMotor, 0, 100);
      rightMotor = constrain(rightMotor, 0, 100);      
      A1PWM = 0;
      A2PWM = map(leftMotor, 0, 100, 0, 255);
      B1PWM = 0;
      B2PWM = map(rightMotor, 0, 100, 0, 255);      
    }
    else if(isForward > 0 && isTurn < 0){
      leftMotor = abs(isForward);
      rightMotor = abs(isForward) + abs(isTurn);
      leftMotor = constrain(leftMotor, 0, 100);
      rightMotor = constrain(rightMotor, 0, 100);
      A1PWM = 0;
      A2PWM = map(leftMotor, 0, 100, 0, 255);
      B1PWM = 0;        
      B2PWM = map(rightMotor, 0, 100, 0, 255);
    }
  }
}

void printMotorValues(){
  Serial.print(A1PWM);
  Serial.print(A2PWM);
  Serial.print(B1PWM);
  Serial.println(B2PWM);
}

void runMotor(){
  calcMotor();
  //printMotorValues();
  analogWrite(MotorA1, A1PWM);
  analogWrite(MotorA2, A2PWM);
  analogWrite(MotorB1, B1PWM);
  analogWrite(MotorB2, B2PWM);  
} 