#include <Arduino.h>

#include <math.h>
#include "mymotor.h"

int DegreeInRadian(double x){
  return x * (3.141592653589/180);;
}

void calcMotor(){
  int isForward = (int)(rawIntData[2] * sin(DegreeInRadian(rawIntData[3])));
  int isTurn = (int)(rawIntData[2] * cos(DegreeInRadian(rawIntData[3])));

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

    if(isForward > 0){
      A1PWM = map(isForward, 0, 100, 0, 255);
      A2PWM = 0;
      B1PWM = map(isForward, 0, 100, 0, 255);
      B2PWM = 0;      
    }
    else if(isForward < 0){
      A1PWM = 0;
      A2PWM = map(abs(isForward), 0, 100, 0, 255);
      B1PWM = 0;
      B2PWM = map(abs(isForward), 0, 100, 0, 255);      
    }

    
    if (isTurn > 0){
      B1PWM = map(isTurn, 0, rawIntData[2], 0, 255);
      B2PWM = 0;
    }
    else if(isTurn < 0){
      B1PWM = 0;
      B2PWM = map(abs(isTurn), 0, rawIntData[2], 0, 255);
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
  analogWrite(MotorA1, A1PWM);
  analogWrite(MotorA2, A2PWM);
  analogWrite(MotorB1, B1PWM);
  analogWrite(MotorB2, B2PWM);  
}