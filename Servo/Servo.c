#include "servo.h"

static TIM_HandleTypeDef *servo_htim;

/* ===== Khoi tao servo =====
   
*/
void Servo_Init(TIM_HandleTypeDef *htim)
{
    servo_htim = htim;

    // PB7 - TIM4_CH2
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
}

/* ===== Ghi goc servo =====
   angle: 0 - 180 do
*/
void Servo_Write(int angle)
{
    if (angle < 0)   angle = 0;
    if (angle > 180) angle = 180;

    /* Cong thuc chuyen goc sang do rong xung
       0 do   -> 500 us
       180 do -> 2500 us
    */
    uint16_t pulse = 500 + (angle * 2000) / 180;

    /* Ghi do rong xung vao thanh ghi CCR */
    __HAL_TIM_SET_COMPARE(servo_htim, TIM_CHANNEL_2, pulse);
}

/*
Ham __HAL_TIM_SET_COMPARE(htim, channel, compare_value)

1. htim          : con tro toi cau truc timer dang su dung
2. channel       : kenh PWM (TIM_CHANNEL_2)
3. compare_value : gia tri CCR dung de tao do rong xung

Vi du cau hinh:
- ARR = 19999
- PSC = 63

=> 1 don vi CCR = 1 us

Goc 0 do   : CCR = 500  -> xung 0.5 ms
Goc 90 do  : CCR = 1500 -> xung 1.5 ms
Goc 180 do : CCR = 2500 -> xung 2.5 ms
*/
