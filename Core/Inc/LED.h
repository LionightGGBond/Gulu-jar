#ifndef __LED_H
#define __LED_H

#include "main.h" // 必须包含，为了引入 CubeMX 生成的引脚宏

/* 定义 LED 的系统状态枚举 */
typedef enum {
    SYS_STATUS_OFF = 0,   // 全灭
    SYS_STATUS_SAFE,      // 安全 (绿灯)
    SYS_STATUS_WARNING,   // 风险 (黄灯)
    SYS_STATUS_DANGER     // 高危 (红灯)
} System_LED_Status;

/* 函数声明 */
void LED_SetStatus(System_LED_Status status);

#endif /* __LED_H */
