
#ifndef INC_AHT21_ENS160_H_
#define INC_AHT21_ENS160_H_



#endif /* INC_AHT21_ENS160_H_ */
#ifndef __AHT21_ENS160_H
#define __AHT21_ENS160_H

#include "main.h"

// 根据之前确认的硬件连线，定义 I2C 地址 (注意 HAL 库需要将 7位地址 左移 1 位)
#define AHT21_ADDR  (0x38 << 1)
#define ENS160_ADDR (0x52 << 1) // 假设你的 ADD 引脚接了 3.3V，若是接 GND 则为 0x52<<1

// 定义数据结构体，用于保存所有环境数据
typedef struct {
    float Temperature;  // 温度 (℃)
    float Humidity;     // 湿度 (%RH)
    uint16_t AQI;       // 空气质量指数 (ENS160 输出)
    uint16_t TVOC;      // 总挥发性有机物 ppb (ENS160 输出)
    uint16_t eCO2;      // 二氧化碳当量 ppm (ENS160 输出)
} AHTMESSAGE;

// 声明外部结构体变量
extern AHTMESSAGE EnvData;

// 函数声明
void EnvSensor_Init(I2C_HandleTypeDef *hi2c);
void EnvSensor_Read_Data(I2C_HandleTypeDef *hi2c, AHTMESSAGE *msg);

#endif
