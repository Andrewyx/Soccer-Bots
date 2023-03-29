#ifndef MYMOTOR_H
#define MYMOTOR_H

#include <Arduino.h>

extern const int MotorA1;
extern const int MotorA2;
extern const int MotorB1;
extern const int MotorB2;

extern bool isMoving;
extern int A1PWM, A2PWM, B1PWM, B2PWM;
extern int rawIntData[4];

int DegreeInRadian(double x);
void calcMotor();
void printMotorValues();
void runMotor();

#endif