#ifndef FLAME_H
#define FLAME_H

#include "main.h"
#include <stdint.h>


#define FL_DO_LEFT_PORT     GPIOA
#define FL_DO_LEFT_PIN      GPIO_PIN_7   // PA7

#define FL_DO_CENTER_PORT   GPIOB
#define FL_DO_CENTER_PIN    GPIO_PIN_0   // PB0

#define FL_DO_RIGHT_PORT    GPIOB
#define FL_DO_RIGHT_PIN     GPIO_PIN_1   // PB1


#define FLAME_DO_ACTIVE_LOW  1

void Flame_Init(void);

uint8_t Flame_LeftDetected_DO(void);
uint8_t Flame_CenterDetected_DO(void);
uint8_t Flame_RightDetected_DO(void);

#endif // FLAME_H
