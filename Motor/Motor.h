#ifndef MOTOR_H
#define MOTOR_H

#include "main.h"
#include <stdint.h>

void Motor_Init(TIM_HandleTypeDef *htim);

void Motor_Forward(uint8_t spd);
void Motor_Backward(uint8_t spd);
void Motor_Left(uint8_t spd);
void Motor_Right(uint8_t spd);

void Motor_Turn_Left(uint8_t speed, float angle);
void Motor_Turn_Right(uint8_t speed, float angle);

void Motor_Stop(void);

#endif // MOTOR_H
