#ifndef MYULTRASONIC_H
#define MYULTRASONIC_H

void initUltrasonic();
void runUltrasonic();
float meanDistance(float dRight, float dLeft);
double leftKalman(double U);
double rightKalman(double U);
double midLineKalman(double U);
double frontKalman(double U);
float midPointVal(float val1, float val2);
float linearReg(float inputVal); 


#endif