#include "motor.h"
#include "main.h"
#include "servo.h"   


#define IN1_PIN  GPIO_PIN_2
#define IN2_PIN  GPIO_PIN_3
#define IN3_PIN  GPIO_PIN_4
#define IN4_PIN  GPIO_PIN_5
#define IN_PORT  GPIOA

static TIM_HandleTypeDef *motor_htim;

void Motor_Init(TIM_HandleTypeDef *htim)
{
    motor_htim = htim;
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_2);
}

static void Motor_SetSpeed(uint8_t left, uint8_t right)
{
    uint32_t arr = motor_htim->Instance->ARR;
    __HAL_TIM_SET_COMPARE(motor_htim, TIM_CHANNEL_1, left  * arr / 100);
    __HAL_TIM_SET_COMPARE(motor_htim, TIM_CHANNEL_2, right * arr / 100);
}

void Motor_Forward(uint8_t spd)
{
    HAL_GPIO_WritePin(IN_PORT, IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN_PORT, IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN_PORT, IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN_PORT, IN4_PIN, GPIO_PIN_RESET);
    Motor_SetSpeed(spd, spd);
}

void Motor_Backward(uint8_t spd)
{
    HAL_GPIO_WritePin(IN_PORT, IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN_PORT, IN2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN_PORT, IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN_PORT, IN4_PIN, GPIO_PIN_SET);
    Motor_SetSpeed(spd, spd);
}

void Motor_Left(uint8_t spd)
{
    HAL_GPIO_WritePin(IN_PORT, IN1_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN_PORT, IN2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN_PORT, IN3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN_PORT, IN4_PIN, GPIO_PIN_RESET);
    Motor_SetSpeed(spd, spd);
}

void Motor_Right(uint8_t spd)
{
    HAL_GPIO_WritePin(IN_PORT, IN1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN_PORT, IN2_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN_PORT, IN3_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN_PORT, IN4_PIN, GPIO_PIN_SET);
    Motor_SetSpeed(spd, spd);
}

void Motor_Turn_Left(uint8_t speed, float angle)
{
    uint32_t time_ms = (uint32_t)((angle / 150.0f) * 1000.0f);
    Motor_Left(speed);
    HAL_Delay(time_ms);
    Motor_Stop();
}

void Motor_Turn_Right(uint8_t speed, float angle)
{
    uint32_t time_ms = (uint32_t)((angle / 150.0f) * 1000.0f);
    Motor_Right(speed);
    HAL_Delay(time_ms);
    Motor_Stop();
}

void Motor_Stop(void)
{
    HAL_GPIO_WritePin(IN_PORT, IN1_PIN|IN2_PIN|IN3_PIN|IN4_PIN, GPIO_PIN_RESET);
    __HAL_TIM_SET_COMPARE(motor_htim, TIM_CHANNEL_1, 0);
    __HAL_TIM_SET_COMPARE(motor_htim, TIM_CHANNEL_2, 0);
}
