#include "pump.h"

/* ===== GPIO DEFINE ===== */
#define PUMP_PORT GPIOA
#define PUMP_PIN  GPIO_PIN_6

void Pump_Init(void)
{
    Pump_Off();   
}

void Pump_On(void)
{
    HAL_GPIO_WritePin(PUMP_PORT, PUMP_PIN, GPIO_PIN_SET);
}

void Pump_Off(void)
{
    HAL_GPIO_WritePin(PUMP_PORT, PUMP_PIN, GPIO_PIN_RESET);
}
