
#ifndef INC_PH_SENSOR_H_
#define INC_PH_SENSOR_H_

#include "main.h"

#define PH_ADC_RESOLUTION    4095.0f    // 12位 ADC 的最大值
#define PH_SYSTEM_VREF       3.3f       // 单片机 ADC 的参考电压

#define PH_CURVE_SLOPE       -5.8887f   // 曲线斜率 (k)
#define PH_CURVE_INTERCEPT   22.000f    // 曲线截距 (b)

float PH_Get_Final_Value(void);//负责收集最终数据
float PH_Get_Final_Value(void);

#endif /* INC_PH_SENSOR_H_ */
