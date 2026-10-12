#include "led.h"

/**
 * @brief  统一控制系统状态指示灯
 * @param  status: 期望设定的发酵系统状态
 * @note   该函数内部自带互斥逻辑，切换新状态前会先清理旧状态。
 *         假设硬件为高电平点亮 (SET = 亮, RESET = 灭)
 */
void LED_SetStatus(System_LED_Status status)
{
    // 第一步：先将三个灯全部熄灭 (RESET)
    // 这样做可以确保无论之前是什么状态，都不会发生两个灯同时亮起的冲突
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

    // 第二步：根据传入的状态，点亮对应的单色灯 (SET)
    switch (status) {
        case SYS_STATUS_SAFE:      // 适宜食用 -> 绿灯
            HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
            break;

        case SYS_STATUS_WARNING:   // 有风险 -> 黄灯
            HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
            break;

        case SYS_STATUS_DANGER:    // 高危险 -> 红灯
            HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
            break;

        case SYS_STATUS_OFF:       // 待机/全灭状态
        default:
            // 已经在第一步全部熄灭，此处无需任何操作
            break;
    }
}
