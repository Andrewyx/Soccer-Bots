#ifndef MYMOTOR_H
#define MYMOTOR_H

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
void initL298N();
void initServoLib();
void runButlerMotor();

#endif