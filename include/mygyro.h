#ifndef MYGYRO_H
#define MYGYRO_H

#include <Arduino.h>

void printGYRO();
void kalman_1d(float KalmanState, float KalmanUncertainty, float KalmanInput, float KalmanMeasurement);
void gyro_signals(void);
void initgyro();
void rungyro();

#endif