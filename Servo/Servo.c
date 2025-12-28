#include "servo.h"

static TIM_HandleTypeDef *servo_htim;

void Servo_Init(TIM_HandleTypeDef *htim)
{
    servo_htim = htim;

    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
}


void Servo_Write(int angle)
{
    if (angle < 0)   angle = 0;
    if (angle > 180) angle = 180;

    uint16_t pulse = 500 + (angle * 2000) / 180;

    __HAL_TIM_SET_COMPARE(servo_htim, TIM_CHANNEL_2, pulse);
}

