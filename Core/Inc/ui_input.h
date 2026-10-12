#ifndef __UI_INPUT_H
#define __UI_INPUT_H

#include "kk_ui.h"

/* 将三个 GPIO 按键归一化为 KK_UI 输入。
 *
 * 硬件接线（按键另一端接 GND，使用 MCU 内部上拉，按下为低电平）：
 *   上键   UP   -> PB3
 *   下键   DOWN -> PB4
 *   确定键 OK   -> PB5
 * 注意：PB3/PB4 默认被 JTAG 占用，UI_InputInit() 会关闭 JTAG（保留 SWD）。
 * 若实际接线不同，只需修改 ui_input.c 顶部的引脚宏。 */

/** 配置三个按键引脚为输入上拉。在 OLED/KK_UI 初始化前调用一次。 */
void UI_InputInit(void);

/** 采样当前按键状态，返回 KK_UI 需要的归一化输入。 */
KK_UI_Input UI_InputRead(void);

#endif /* __UI_INPUT_H */
