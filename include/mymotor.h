#ifndef MYMOTOR_H
#define MYMOTOR_H

extern bool isMoving;
extern int A1PWM, A2PWM, B1PWM, B2PWM;
extern int rawIntData[4];
extern int lineCollData[2];


int DegreeInRadian(double x);
void calcMotor();
void printMotorValues();
void runMotor();
void initL298N();
void initServoLib();
void runButlerMotor();
void runMotorWithLines();

#endif