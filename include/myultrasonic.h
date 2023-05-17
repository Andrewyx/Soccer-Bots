#ifndef MYULTRASONIC_H
#define MYULTRASONIC_H

void initUltrasonic();
void runUltrasonic();
void meanDistance();
double leftKalman(double U);
double rightKalman(double U);
double midLineKalman(double U);
double midPointVal(double val1, double val2);


#endif