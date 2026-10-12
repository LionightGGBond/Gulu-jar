#include "kk_ui_app.h"

#include "AHT21_ENS160.h"
#include "conductivity sensor.h"
#include "kk_home_icons.h"
#include "kk_oled.h"
#include "kk_ui_font.h"
#include "tim.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

/* 页面 ID：路由数组第 0 项对应页面 ID 1，ID 0 无效。 */
enum {
    PAGE_HOME = 1,        /* 图标首页 */
    PAGE_MENU_MONITOR,    /* 数据监测菜单 */
    PAGE_INFO_OVERVIEW,   /* 实时总览 */
    PAGE_INFO_ENV,        /* 环境数据 */
    PAGE_INFO_WATER,      /* 水质数据 */
    PAGE_INFO_FAN,        /* 风扇转速 */
    PAGE_INFO_ABOUT       /* 关于设备 */
};

/* 业务数据由 main.c 的采集循环维护。 */
extern float current_ph_value;

/* 数值字符串的刷新周期（不超过 1s）。 */
#define APP_REFRESH_PERIOD_MS 500U
/* 即使数值没变化，也至少按此周期强制重画一次信息页。 */
#define APP_FORCE_REFRESH_MS  1000U

static char temp_text[12];
static char hum_text[12];
static char co2_text[12];
static char ph_text[12];
static char ec_text[16];
static char fan_text[12];
static char fan_state_text[12];
static char water_state_text[12];
static char screen_text[12];
static uint32_t last_refresh_at;
static uint32_t last_force_at;

static const KK_UI_HomeItem home_items[] = {
    {"数据监测", home_icons[0], PAGE_MENU_MONITOR},
    {"实时总览", home_icons[2], PAGE_INFO_OVERVIEW},
    {"关于设备", home_icons[3], PAGE_INFO_ABOUT}
};

static const KK_UI_MenuItem monitor_items[] = {
    {"环境数据", KK_UI_MENU_PAGE, PAGE_INFO_ENV},
    {"水质数据", KK_UI_MENU_PAGE, PAGE_INFO_WATER},
    {"风扇转速", KK_UI_MENU_PAGE, PAGE_INFO_FAN}
};

static const KK_UI_InfoRow overview_rows[] = {
    {"温度", temp_text},
    {"湿度", hum_text},
    {"CO2", co2_text},
    {"pH值", ph_text},
    {"电导率", ec_text},
    {"风扇转速", fan_text}
};

static const KK_UI_InfoRow env_rows[] = {
    {"温度", temp_text},
    {"湿度", hum_text},
    {"CO2", co2_text}
};

static const KK_UI_InfoRow water_rows[] = {
    {"pH值", ph_text},
    {"电导率", ec_text},
    {"状态", water_state_text}
};

static const KK_UI_InfoRow fan_rows[] = {
    {"转速", fan_text},
    {"状态", fan_state_text}
};

static const KK_UI_InfoRow about_rows[] = {
    {"设备名称", "Gulu"},
    {"固件版本", "1.0"},
    {"显示驱动", "KK_OLED"},
    {"屏幕尺寸", screen_text},
    {"刷新方式", "IT"},
    {"按键操作", "上下/确定"}
};

static const KK_UI_HomePage home_pages[] = {
    {home_items, (uint16_t)(sizeof(home_items) / sizeof(home_items[0]))}
};

static const KK_UI_MenuPage menu_pages[] = {
    {"数据监测", monitor_items, NULL,
     (uint16_t)(sizeof(monitor_items) / sizeof(monitor_items[0]))}
};

static const KK_UI_InfoPage info_pages[] = {
    {"实时总览", overview_rows,
     (uint16_t)(sizeof(overview_rows) / sizeof(overview_rows[0]))},
    {"环境数据", env_rows,
     (uint16_t)(sizeof(env_rows) / sizeof(env_rows[0]))},
    {"水质数据", water_rows,
     (uint16_t)(sizeof(water_rows) / sizeof(water_rows[0]))},
    {"风扇转速", fan_rows,
     (uint16_t)(sizeof(fan_rows) / sizeof(fan_rows[0]))},
    {"关于设备", about_rows,
     (uint16_t)(sizeof(about_rows) / sizeof(about_rows[0]))}
};

static const KK_UI_PageRoute routes[] = {
    {KK_UI_PAGE_HOME, 0U},   /* PAGE_HOME */
    {KK_UI_PAGE_MENU, 0U},   /* PAGE_MENU_MONITOR */
    {KK_UI_PAGE_INFO, 0U},   /* PAGE_INFO_OVERVIEW */
    {KK_UI_PAGE_INFO, 1U},   /* PAGE_INFO_ENV */
    {KK_UI_PAGE_INFO, 2U},   /* PAGE_INFO_WATER */
    {KK_UI_PAGE_INFO, 3U},   /* PAGE_INFO_FAN */
    {KK_UI_PAGE_INFO, 4U}    /* PAGE_INFO_ABOUT */
};

static const KK_UI_App app = {
    .root_page = PAGE_HOME,
    .routes = routes,
    .route_count = (uint16_t)(sizeof(routes) / sizeof(routes[0])),
    .home_pages = home_pages,
    .home_page_count = (uint16_t)(sizeof(home_pages) / sizeof(home_pages[0])),
    .menu_pages = menu_pages,
    .menu_page_count = (uint16_t)(sizeof(menu_pages) / sizeof(menu_pages[0])),
    .info_pages = info_pages,
    .info_page_count = (uint16_t)(sizeof(info_pages) / sizeof(info_pages[0])),
    .custom_page_count = 0U,
    .int_bindings = NULL,
    .int_binding_count = 0U,
    .bool_bindings = NULL,
    .bool_binding_count = 0U,
    .confirm_descs = NULL,
    .confirm_desc_count = 0U,
    .fonts = {kk_font_ui_home, kk_font_ui_title, kk_font_ui_body},
    .texts = {"返回", "取消", "确定", "开", "关", "提示"}
};

/** 只有内容变化时才写入并返回 true，避免无意义重绘。 */
static bool app_text_changed(char *dst, size_t capacity, const char *value)
{
    if (strncmp(dst, value, capacity) == 0) {
        return false;
    }
    strncpy(dst, value, capacity - 1U);
    dst[capacity - 1U] = '\0';
    return true;
}

/** 采集一次传感器数据并刷新各显示字符串。 */
static void app_refresh_values(void)
{
    char buffer[24];
    bool changed = false;

    snprintf(buffer, sizeof(buffer), "%.1f C", (double)EnvData.Temperature);
    changed = app_text_changed(temp_text, sizeof(temp_text), buffer) || changed;

    snprintf(buffer, sizeof(buffer), "%.1f %%", (double)EnvData.Humidity);
    changed = app_text_changed(hum_text, sizeof(hum_text), buffer) || changed;

    snprintf(buffer, sizeof(buffer), "%u ppm", (unsigned)EnvData.eCO2);
    changed = app_text_changed(co2_text, sizeof(co2_text), buffer) || changed;

    snprintf(buffer, sizeof(buffer), "%.2f", (double)current_ph_value);
    changed = app_text_changed(ph_text, sizeof(ph_text), buffer) || changed;

    if (Cond_State == COND_STATE_ERROR) {
        changed = app_text_changed(ec_text, sizeof(ec_text), "错误") || changed;
    } else {
        snprintf(buffer, sizeof(buffer), "%uus/cm", (unsigned)E_message);
        changed = app_text_changed(ec_text, sizeof(ec_text), buffer) || changed;
    }
    changed = app_text_changed(water_state_text, sizeof(water_state_text),
                               Cond_State == COND_STATE_ERROR ? "错误" : "正常")
              || changed;

    /* 风扇转速：直接读取 TIM2 通道 1 最后写入的 PWM 比较值(CCR1)，
     * 按 ARR+1 计算占空比百分比，即真正输出给风扇的驱动数值。 */
    {
        uint32_t arr = __HAL_TIM_GET_AUTORELOAD(&htim2) + 1U;
        uint32_t ccr = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_1);
        unsigned fan_percent = 0U;

        if (arr != 0U) {
            fan_percent = (unsigned)((ccr * 100U + arr / 2U) / arr);
        }
        if (fan_percent > 100U) {
            fan_percent = 100U;
        }
        snprintf(buffer, sizeof(buffer), "%u %%", fan_percent);
        changed = app_text_changed(fan_text, sizeof(fan_text), buffer) || changed;
        changed = app_text_changed(fan_state_text, sizeof(fan_state_text),
                                   (ccr > 0U) ? "运行" : "停止") || changed;
    }

    if (changed) {
        KK_UI_Invalidate();
    }
}

const KK_UI_App *KK_UI_AppGet(void)
{
    return &app;
}

void KK_UI_AppReset(uint32_t now_ms)
{
    snprintf(screen_text, sizeof(screen_text), "%ux%u",
             (unsigned)OLED_GetWidth(), (unsigned)OLED_GetHeight());

    temp_text[0] = '\0';
    hum_text[0] = '\0';
    co2_text[0] = '\0';
    ph_text[0] = '\0';
    ec_text[0] = '\0';
    fan_text[0] = '\0';
    fan_state_text[0] = '\0';
    water_state_text[0] = '\0';

    last_refresh_at = now_ms - APP_REFRESH_PERIOD_MS;
    last_force_at = now_ms;
    app_refresh_values();
}

void KK_UI_AppProcessEvents(void)
{
    KK_UI_EventId event;

    /* 当前界面只展示只读数据，没有业务事件需要处理。 */
    while (KK_UI_PollEvent(&event)) {
        (void)event;
    }
}

void KK_UI_AppUpdate(uint32_t now_ms)
{
    if (now_ms - last_refresh_at >= APP_REFRESH_PERIOD_MS) {
        last_refresh_at = now_ms;
        app_refresh_values();
    }
    /* 即使数值未变化，也保证信息页至少每 1s 重画一次（无差异时不会产生总线流量）。 */
    if (now_ms - last_force_at >= APP_FORCE_REFRESH_MS) {
        last_force_at = now_ms;
        KK_UI_Invalidate();
    }
}
