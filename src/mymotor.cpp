#include <Arduino.h>

#include <math.h>
#include "mymotor.h"

int leftMotor, rightMotor;
int isForward, isTurn;

int DegreeInRadian(double x){
  return x * (3.1415/180);;
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