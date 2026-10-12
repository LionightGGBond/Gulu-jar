#include "Conductivity sensor.h"
#include "usart.h"

// 全局变量：供 main.c 读取
float T_message = 0.0f;
uint16_t E_message = 0;

// 状态机与接收缓存
volatile Cond_State_t Cond_State = COND_STATE_IDLE;
uint8_t Rx_Buffer[6] = {0};

// 获取电导率指令: 格式为 命令(1B)+参数(4B)+校验和(1B)
uint8_t Measure_Cmd[6] = {0xA0, 0x00, 0x00, 0x00, 0x00, 0xA0};

void Conductivity_Init(void)
{
    HAL_Delay(200); // 确保传感器上电稳定
    Cond_State = COND_STATE_IDLE;
}

// 触发测量（非阻塞）
void Conductivity_StartMeasure_IT(void)
{
    if (Cond_State == COND_STATE_IDLE || Cond_State == COND_STATE_ERROR)
    {
        __HAL_UART_CLEAR_OREFLAG(&huart2); // 清除溢出错误标志
        Cond_State = COND_STATE_BUSY_RX;

        // 开启接收中断，等待6字节返回
        HAL_UART_Receive_IT(&huart2, Rx_Buffer, 6);
        // 发送A0检测指令
        HAL_UART_Transmit_IT(&huart2, Measure_Cmd, 6);
    }
}

// 数据解析处理（在 main 的 while 循环中调用）
void Conductivity_Process(void)
{
    if (Cond_State == COND_STATE_DATA_READY)
    {
        // 校验和计算
        uint8_t check_sum = 0;
        for (uint8_t i = 0; i < 5; i++) check_sum += Rx_Buffer[i];

        // 判断帧头 AA 和校验和是否正确
        if (Rx_Buffer[0] != 0xAA || Rx_Buffer[5] != check_sum)
        {
            Cond_State = COND_STATE_ERROR;
            return;
        }

        // 解析电导率 (uS/cm)
        E_message = (Rx_Buffer[1] << 8) | Rx_Buffer[2];

        // 解析温度 (真实温度 = 原始值 / 100)
        uint16_t temp_raw = (Rx_Buffer[3] << 8) | Rx_Buffer[4];
        T_message = temp_raw / 100.0f;

        Cond_State = COND_STATE_IDLE; // 解析成功，重置状态
    }
}
