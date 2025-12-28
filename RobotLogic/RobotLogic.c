#include "RobotLogic.h"
#include "hcsr04.h"
#include "servo.h"
#include "motor.h"
#include "main.h"

#define SERVO_GIUA    90
#define SERVO_PHAI    10
#define SERVO_TRAI    170

#define KC_THOANG_BEN     20.0f
#define KC_NE_VATCAN      28.0f
#define KC_PHANH_DUNG     16.0f
#define KC_DUNG_LUA       16.0f

#define TG_LUI_NHE        160
#define TG_RE_TRAI        820
#define TG_RE_PHAI        820
#define TG_CHO_SERVO      420

#define TG_PHUN           3000
#define TG_TIEP_CAN_LUA   1500

#define TD_DI_THANG       45
#define TD_TIEP_CAN       25
#define TD_LUI            30
#define TD_RE             65

#define LUA_TRAI_PORT     GPIOA
#define LUA_TRAI_PIN      GPIO_PIN_7

#define LUA_GIUA_PORT     GPIOB
#define LUA_GIUA_PIN      GPIO_PIN_0

#define LUA_PHAI_PORT     GPIOB
#define LUA_PHAI_PIN      GPIO_PIN_1

#define BOM_PORT          GPIOA
#define BOM_PIN           GPIO_PIN_6

static void Bom_ON(void)
{
    HAL_GPIO_WritePin(BOM_PORT, BOM_PIN, GPIO_PIN_SET);
}

static void Bom_OFF(void)
{
    HAL_GPIO_WritePin(BOM_PORT, BOM_PIN, GPIO_PIN_RESET);
}

static float Do_KhoangCach_Min(uint8_t lan)
{
    float d, mn = 1000.0f;

    while (lan--)
    {
        d = HCSR04_Read();
        if (d > 1.0f && d < 400.0f && d < mn) mn = d;
        HAL_Delay(10);
    }

    return (mn == 1000.0f) ? -1.0f : mn;
}

static float Quet_KhoangCach(uint8_t goc)
{
    Servo_Write(goc);
    HAL_Delay(TG_CHO_SERVO);
    return Do_KhoangCach_Min(2);
}

static uint8_t DocLua_1Lan(GPIO_TypeDef* port, uint16_t pin)
{
    return (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET) ? 1u : 0u;
}

static uint8_t DocLua_OnDinh(GPIO_TypeDef* port, uint16_t pin)
{
    uint8_t cnt = 0;
    for (int i = 0; i < 3; i++)
    {
        cnt += DocLua_1Lan(port, pin);
        HAL_Delay(3);
    }
    return (cnt >= 2);
}

static uint8_t LuaGiua_OnDinh_300ms(void)
{
    uint32_t t0 = HAL_GetTick();
    while (HAL_GetTick() - t0 < 300)
    {
        if (!DocLua_1Lan(LUA_GIUA_PORT, LUA_GIUA_PIN)) return 0;
        HAL_Delay(10);
    }
    return 1;
}

static void Xe_ReTrai(void)
{
    Motor_Left(TD_RE);
    HAL_Delay(TG_RE_TRAI);
    Motor_Stop();
    HAL_Delay(40);
}

static void Xe_RePhai(void)
{
    Motor_Right(TD_RE);
    HAL_Delay(TG_RE_PHAI);
    Motor_Stop();
    HAL_Delay(40);
}

static void Xe_LuiNhe(void)
{
    Motor_Stop();
    HAL_Delay(50);

    Motor_Backward(TD_LUI);
    HAL_Delay(TG_LUI_NHE);

    Motor_Stop();
    HAL_Delay(40);
}

static void Xe_XuLy_VatCan_Truoc(void)
{
    Servo_Write(SERVO_GIUA);
    HAL_Delay(60);

    float truoc = Do_KhoangCach_Min(3);
    if (truoc < 0.0f) return;

    if (truoc <= KC_PHANH_DUNG)
    {
        Motor_Stop();
        HAL_Delay(30);
        Xe_LuiNhe();
        HAL_Delay(30);
    }

    if (truoc > KC_NE_VATCAN) return;

    Motor_Stop();
    HAL_Delay(20);

    float benPhai = Quet_KhoangCach(SERVO_PHAI);
    float benTrai = Quet_KhoangCach(SERVO_TRAI);

    Servo_Write(SERVO_GIUA);
    HAL_Delay(30);

    uint8_t phaiOk = (benPhai > KC_THOANG_BEN);
    uint8_t traiOk = (benTrai > KC_THOANG_BEN);

    if (phaiOk && !traiOk) { Xe_RePhai(); return; }
    if (!phaiOk && traiOk) { Xe_ReTrai(); return; }

    if (phaiOk && traiOk)
    {
        if (benPhai >= benTrai) Xe_RePhai();
        else Xe_ReTrai();
        return;
    }

    Motor_Backward(TD_LUI);
    HAL_Delay(250);
    Motor_Stop();
    HAL_Delay(30);
    Xe_RePhai();
}

static void Xe_PhunNuoc(void)
{
    uint32_t t0 = HAL_GetTick();
    Bom_ON();

    while (HAL_GetTick() - t0 < TG_PHUN)
    {
        HAL_Delay(20);
    }

    Bom_OFF();
}

static void Xe_TiepCan_Lua(void)
{
    uint32_t t0 = HAL_GetTick();
    uint32_t tMove = HAL_GetTick();

    Servo_Write(SERVO_GIUA);
    HAL_Delay(60);

    while (1)
    {
        uint32_t now = HAL_GetTick();

        if (now - t0 > 6000)
        {
            Motor_Stop();
            Bom_OFF();
            return;
        }

        if (!DocLua_1Lan(LUA_GIUA_PORT, LUA_GIUA_PIN))
        {
            Motor_Stop();
            Bom_OFF();
            return;
        }

        float d = Do_KhoangCach_Min(2);

        if (d >= 0.0f && d <= KC_DUNG_LUA)
        {
            Motor_Stop();
            HAL_Delay(80);
            if (LuaGiua_OnDinh_300ms()) Xe_PhunNuoc();
            return;
        }

        if (now - tMove > TG_TIEP_CAN_LUA)
        {
            Motor_Stop();
            HAL_Delay(80);
            if (LuaGiua_OnDinh_300ms()) Xe_PhunNuoc();
            return;
        }

        Motor_Forward(TD_TIEP_CAN);
        HAL_Delay(30);
    }
}

void Xe_TranhVat_UuTienLua(void)
{
    Xe_XuLy_VatCan_Truoc();

    uint8_t luaTrai = DocLua_OnDinh(LUA_TRAI_PORT, LUA_TRAI_PIN);
    uint8_t luaGiua = DocLua_OnDinh(LUA_GIUA_PORT, LUA_GIUA_PIN);
    uint8_t luaPhai = DocLua_OnDinh(LUA_PHAI_PORT, LUA_PHAI_PIN);

    if (luaGiua)
    {
        Xe_TiepCan_Lua();
        return;
    }

    if (luaTrai && !luaPhai)
    {
        Motor_Left(TD_RE);
        HAL_Delay(100);
        return;
    }

    if (!luaTrai && luaPhai)
    {
        Motor_Right(TD_RE);
        HAL_Delay(100);
        return;
    }

    Motor_Forward(TD_DI_THANG);
    HAL_Delay(60);
}
