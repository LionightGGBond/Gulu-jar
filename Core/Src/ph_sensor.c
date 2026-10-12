#include "ph_sensor.h"
extern ADC_HandleTypeDef hadc1;

void Ph_sensor_init(void)
{
    HAL_ADCEx_Calibration_Start(&hadc1);
    HAL_Delay(5);
}

static uint32_t PH_Get_Filtered_ADC(void)
{
		uint32_t adc_buffer[10];
	    uint32_t temp;
	    uint32_t sum = 0;

	    // 1. 启动 ADC
	 HAL_ADC_Start(&hadc1);
	 	//冒泡算法降噪
	 for(uint8_t i = 0; i < 10; i++)
 {
		 if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
	{
		 adc_buffer[i] = HAL_ADC_GetValue(&hadc1);
  	}
 }
	 // 3. 停止 ADC，节省功耗 (如果不考虑功耗，也可以不停止，让它一直转)
	     HAL_ADC_Stop(&hadc1);
	 // 4. 冒泡排序 (从小到大)
	for(uint8_t i = 0; i < 9; i++){
		for(uint8_t j = 0; j < 9 - i; j++){

		if (adc_buffer[j] > adc_buffer[j+1]){
			temp = adc_buffer[j+1];
			adc_buffer[j+1] = adc_buffer[j];
			adc_buffer[j] = temp;
			}
		}
	}
	for(uint8_t i = 1; i < 8;i++){
		sum = sum + adc_buffer[i];
	}
	return (sum / 6);
}

//最终输出检测
float PH_Get_Final_Value(void){
	float voltage = 0.0f;
	float ph_value = 0.0f;

		// 1. 获取滤波后的 ADC 原码
	    uint32_t adc_raw = PH_Get_Filtered_ADC();

	    // 2. 转换为实际电压值
	    voltage = ((float)adc_raw / PH_ADC_RESOLUTION) * PH_SYSTEM_VREF;

	    // 3. 代入公式计算 PH 值
	    ph_value = (PH_CURVE_SLOPE * voltage) + PH_CURVE_INTERCEPT;

	    if(ph_value < 0.0f)
		{
			ph_value = 0.0f;
		}
		else if(ph_value > 14.0f)
		{
			ph_value = 14.0f;
		}

		return ph_value;
 }



