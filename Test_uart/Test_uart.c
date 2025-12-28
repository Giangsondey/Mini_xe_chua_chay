#include "test_uart.h"
#include "hcsr04.h"
#include <stdio.h>
#include <string.h>

/* ================== LUA SENSOR (DO active low) ==================
   Binh thuong: pin = 1
   Co lua:      pin = 0
   Yeu cau: in "co lua : 1" khi pin = 0
*/

/* sua chan lua o day neu can */
#define FLAME_PORT   GPIOA
#define FLAME_PIN    GPIO_PIN_7

static UART_HandleTypeDef *g_huart = NULL;
static uint32_t g_last = 0;

static void uart_send(const char *s)
{
    if (!g_huart) return;
    HAL_UART_Transmit(g_huart, (uint8_t*)s, (uint16_t)strlen(s), HAL_MAX_DELAY);
}

static uint8_t flame_read_once(void)
{
    return (HAL_GPIO_ReadPin(FLAME_PORT, FLAME_PIN) == GPIO_PIN_RESET) ? 1 : 0;
}

static uint8_t flame_read_stable(void)
{
    uint8_t cnt = 0;
    for (int i = 0; i < 5; i++)
    {
        cnt += flame_read_once();
        HAL_Delay(5);
    }
    return (cnt >= 3) ? 1 : 0;
}

void TestUART_Init(UART_HandleTypeDef *huart)
{
    g_huart = huart;
    g_last = HAL_GetTick();
}

void TestUART_Task(uint32_t period_ms)
{
    uint32_t now = HAL_GetTick();
    if ((now - g_last) < period_ms) return;
    g_last = now;

    uint8_t fire = flame_read_stable();
    float dis = HCSR04_Read(); 

    char buf[80];

    snprintf(buf, sizeof(buf), "co lua : %u\r\n", (unsigned)fire);
    uart_send(buf);

    if (dis < 0.0f)
        snprintf(buf, sizeof(buf), "dis: ERR\r\n\r\n");
    else
        snprintf(buf, sizeof(buf), "dis: %.2f\r\n\r\n", dis);

    uart_send(buf);
}
