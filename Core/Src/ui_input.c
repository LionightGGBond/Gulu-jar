#include "ui_input.h"

#include "main.h"

/* 按键引脚映射：按键另一端接 GND，使用内部上拉。 */
#define UI_KEY_PORT       GPIOB
#define UI_KEY_UP_PIN     GPIO_PIN_3
#define UI_KEY_DOWN_PIN   GPIO_PIN_4
#define UI_KEY_OK_PIN     GPIO_PIN_5
#define UI_KEY_ALL_PINS   (UI_KEY_UP_PIN | UI_KEY_DOWN_PIN | UI_KEY_OK_PIN)

/* 临时按键自检：置 1 时用板上三颗 LED 实时显示三个按键引脚电平，
 * 灯亮 = 该脚为低（按下）。用于区分"接线/共地问题"与"固件问题"。
 * 排查完成后请改回 0（否则会占用 LED 的状态指示功能）。 */
#define UI_INPUT_LED_SELFTEST 1

void UI_InputInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /* PB3=JTDO、PB4=NJTRST 默认被 JTAG 复用占用；关闭 JTAG 并保留 SWD 后
     * 这两个引脚即可作为普通 GPIO 使用（PA13/PA14 的 SWD 调试不受影响）。 */
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_AFIO_REMAP_SWJ_NOJTAG();

    GPIO_InitStruct.Pin = UI_KEY_ALL_PINS;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(UI_KEY_PORT, &GPIO_InitStruct);
}

KK_UI_Input UI_InputRead(void)
{
    KK_UI_Input input;
    bool up_low;
    bool down_low;
    bool ok_low;

    /* 按键另一端接 GND，按下时引脚被拉低。 */
    up_low = (HAL_GPIO_ReadPin(UI_KEY_PORT, UI_KEY_UP_PIN) == GPIO_PIN_RESET);
    down_low = (HAL_GPIO_ReadPin(UI_KEY_PORT, UI_KEY_DOWN_PIN) == GPIO_PIN_RESET);
    ok_low = (HAL_GPIO_ReadPin(UI_KEY_PORT, UI_KEY_OK_PIN) == GPIO_PIN_RESET);

    input.keys = 0U;
    input.encoder_delta = 0;

    if (up_low) {
        input.keys |= (uint8_t)KK_UI_KEY_UP;
    }
    if (down_low) {
        input.keys |= (uint8_t)KK_UI_KEY_DOWN;
    }
    if (ok_low) {
        input.keys |= (uint8_t)KK_UI_KEY_OK;
    }

#if UI_INPUT_LED_SELFTEST
    /* 自检模式：红/黄/绿 LED 分别跟随 上/下/确定 引脚，灯亮 = 该脚为低。 */
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin,
                      up_low ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin,
                      down_low ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin,
                      ok_low ? GPIO_PIN_SET : GPIO_PIN_RESET);
#endif

    return input;
}
