#include "delay_us.h"

static TIM_HandleTypeDef *delay_htim;

//----------------------------------------------------------
void Delay_Init(TIM_HandleTypeDef *htim)
{
    delay_htim = htim;
    HAL_TIM_Base_Start(htim);
}

//----------------------------------------------------------
// Hàm delay micro giây
//----------------------------------------------------------
void delay_us(uint16_t us)
{
    if (delay_htim == NULL) return;           
    __HAL_TIM_SET_COUNTER(delay_htim, 0);     
    while (__HAL_TIM_GET_COUNTER(delay_htim) < us);
}

TIM_HandleTypeDef* Delay_GetHandle(void)
{
    return delay_htim;
}
