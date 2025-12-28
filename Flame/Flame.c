#include "flame.h"

void Flame_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* PA7 input */
    GPIO_InitStruct.Pin  = FL_DO_LEFT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;     // n?u b? trôi m?c, d?i GPIO_PULLUP / PULLDOWN
    HAL_GPIO_Init(FL_DO_LEFT_PORT, &GPIO_InitStruct);

    /* PB0 + PB1 input */
    GPIO_InitStruct.Pin  = FL_DO_CENTER_PIN | FL_DO_RIGHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(FL_DO_CENTER_PORT, &GPIO_InitStruct);
}

/* ===== Kiem tra cam bien lua ben trai ===== */
uint8_t Flame_LeftDetected_DO(void)
{
#if FLAME_DO_ACTIVE_LOW
    return (HAL_GPIO_ReadPin(FL_DO_LEFT_PORT, FL_DO_LEFT_PIN) == GPIO_PIN_RESET) ? 1 : 0;
#else
    return (HAL_GPIO_ReadPin(FL_DO_LEFT_PORT, FL_DO_LEFT_PIN) == GPIO_PIN_SET) ? 1 : 0;
#endif
}

/* ===== Kiem tra cam bien lua o giua ===== */
uint8_t Flame_CenterDetected_DO(void)
{
#if FLAME_DO_ACTIVE_LOW
    return (HAL_GPIO_ReadPin(FL_DO_CENTER_PORT, FL_DO_CENTER_PIN) == GPIO_PIN_RESET) ? 1 : 0;
#else
    return (HAL_GPIO_ReadPin(FL_DO_CENTER_PORT, FL_DO_CENTER_PIN) == GPIO_PIN_SET) ? 1 : 0;
#endif
}

/* ===== Kiem tra cam bien lua ben phai ===== */
uint8_t Flame_RightDetected_DO(void)
{
#if FLAME_DO_ACTIVE_LOW
    return (HAL_GPIO_ReadPin(FL_DO_RIGHT_PORT, FL_DO_RIGHT_PIN) == GPIO_PIN_RESET) ? 1 : 0;
#else
    return (HAL_GPIO_ReadPin(FL_DO_RIGHT_PORT, FL_DO_RIGHT_PIN) == GPIO_PIN_SET) ? 1 : 0;
#endif
}
