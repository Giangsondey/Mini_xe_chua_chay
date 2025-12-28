#ifndef TEST_UART_H
#define TEST_UART_H

#include "main.h"

// init: truyen UART muon in (vd: &huart1)
void TestUART_Init(UART_HandleTypeDef *huart);

// task: goi trong while(1), chu ky in theo period_ms
void TestUART_Task(uint32_t period_ms);

#endif
