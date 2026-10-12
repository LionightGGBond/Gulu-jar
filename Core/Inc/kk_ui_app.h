#ifndef KK_UI_APP_H
#define KK_UI_APP_H

#include "kk_ui.h"

#include <stdint.h>

/** 返回应用静态描述表，供 KK_UI_Init() 使用。 */
const KK_UI_App *KK_UI_AppGet(void);

/** 在 OLED_Init() 成功之后、KK_UI_Init() 之前调用，重置应用状态。 */
void KK_UI_AppReset(uint32_t now_ms);

/** 主循环中在 KK_UI_Update() 之后调用，处理业务事件。 */
void KK_UI_AppProcessEvents(void);

/** 主循环中调用，周期刷新传感器数值字符串。 */
void KK_UI_AppUpdate(uint32_t now_ms);

#endif /* KK_UI_APP_H */
