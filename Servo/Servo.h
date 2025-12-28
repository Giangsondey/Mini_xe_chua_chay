#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f1xx_hal.h"

/* ===== Khoi tao servo ===== */
void Servo_Init(TIM_HandleTypeDef *htim);

/* ===== Ghi goc servo (0 - 180 do) ===== */
void Servo_Write(int angle);

#endif /* __SERVO_H */
