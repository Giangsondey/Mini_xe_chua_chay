#ifndef DELAY_US_H
#define DELAY_US_H

#include "main.h"
#include <stdint.h>

void Delay_Init(TIM_HandleTypeDef *htim);
void delay_us(uint16_t us);
TIM_HandleTypeDef* Delay_GetHandle(void);

#endif // DELAY_US_H
