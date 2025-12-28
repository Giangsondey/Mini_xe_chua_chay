#include "hcsr04.h"
#include "delay_us.h"

#define TRIG_PORT GPIOB
#define TRIG_PIN  GPIO_PIN_11   
#define ECHO_PORT GPIOB
#define ECHO_PIN  GPIO_PIN_10   

void HCSR04_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();


    GPIO_InitStruct.Pin = TRIG_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TRIG_PORT, &GPIO_InitStruct);


    GPIO_InitStruct.Pin = ECHO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ECHO_PORT, &GPIO_InitStruct);
}


static void HCSR04_Trigger(void)
{
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
    delay_us(2);
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

float HCSR04_Read(void)
{
    uint32_t timeout = 60000;  
    uint32_t start = 0, stop = 0;
    float distance = -1.0f;

    (void)start; 

    HCSR04_Trigger();

    while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_RESET)
    {
        if (--timeout == 0)
            return -1.0f;
    }

    __HAL_TIM_SET_COUNTER(Delay_GetHandle(), 0);

    timeout = 60000;
    while (HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN) == GPIO_PIN_SET)
    {
        if (--timeout == 0)
            return -1.0f;
    }

    stop = __HAL_TIM_GET_COUNTER(Delay_GetHandle());

    if (stop > 60000) stop = 60000;

    distance = (float)stop / 58.0f;  

    if (distance > 400.0f) distance = 400.0f;

    return distance;
}
