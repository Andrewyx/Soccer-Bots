#include <Arduino.h>
#include <ESP32Servo.h>
#include <math.h>
#include "mymotor.h"
#include "mylinesensor.h"
#include "myultrasonic.h"

const int MotorA1 = 4;
const int MotorB1 = 19;

const int MotorA2 = 18;
const int MotorB2 = 23;

int leftMotor, rightMotor;
int isForward, isTurn;
Servo leftServo, rightServo;

int DegreeInRadian(double x){
  return x * (3.1415/180);;
}

void initServoLib(){
  // 16 servo objects can be created on the ESP32
  // Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33   
  // Allow allocation of all timers
	leftServo.attach(MotorA1, 1000, 2000);
  rightServo.attach(MotorB1, 1000, 2000);  
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	leftServo.setPeriodHertz(50);
  rightServo.setPeriodHertz(50);


}

void runMotorWithLines(){
    readLineSensor();

    if(!lineCollData[0]){
      leftServo.write(70);
      rightServo.write(90);
      delay(1000);
    }
    else if (!lineCollData[1]){
      leftServo.write(90);
      rightServo.write(70);
      delay(1000);
    }
    else{
      leftServo.write(70);
      rightServo.write(70);
    }
}

void runMotorWithUltrasonic(){
    int toleranceVal = 20;
    runUltrasonic();
    if(cleanedLD >= 1000 && cleanedRD >= 1000){
      leftServo.write(70);
      rightServo.write(90);      
    }
    else if(cleanedLD - cleanedRD > toleranceVal){
      leftServo.write(65);
      rightServo.write(70);       
    }
    else if(cleanedLD - cleanedRD < -toleranceVal){
      leftServo.write(70);
      rightServo.write(65); 
    }
    else{
      leftServo.write(70);
      rightServo.write(70);      
    }
}

void runButlerMotor(){
  isForward = (int)(rawIntData[2] * sin(rawIntData[3]*PI/180));
  isTurn = (int)(rawIntData[2] * cos(rawIntData[3]*PI/180));
  
  /*
  Serial.print("Cos: ");
  Serial.print(cos(rawIntData[3]*PI/180));
  Serial.print(" Speed: ");
  Serial.print(rawIntData[2]);
  Serial.print(" Angle: ");
  Serial.print(rawIntData[3]);  
  Serial.print(" ForwardVal: ");
  Serial.print(isForward);
  Serial.print(" TurnVal: ");
  Serial.print(isTurn);
  */
    
  if (rawIntData[2] == 0){
    leftServo.write(90);
    rightServo.write(90);
  }

  else {
      if(isForward > 0){
        leftMotor = isForward + isTurn;
        rightMotor = isForward - isTurn;
      }
      else{
        leftMotor = isForward - isTurn;
        rightMotor = isForward + isTurn;
      }

      leftMotor = constrain(leftMotor, -100, 100);
      rightMotor = constrain(rightMotor, -100, 100);
      leftMotor = map(leftMotor, -100, 100, 60, 130);
      rightMotor = map(rightMotor, -100, 100, 60, 130);
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

  /*
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
  */
    
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