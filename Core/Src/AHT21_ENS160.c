#include "AHT21_ENS160.h"

AHTMESSAGE EnvData = {0};

// 初始化传感器
void EnvSensor_Init(I2C_HandleTypeDef *hi2c)
{
	// ==================== AHT21 初始化 ====================
	    uint8_t aht_init_cmd[3] = {0x71, 0x00, 0x00};
	    // 发送状态查询，若未校准可能需要发送 0xBE 初始化，这里先给一个基础指令防卡死
	    HAL_I2C_Master_Transmit(hi2c, AHT21_ADDR, aht_init_cmd, 1, 100);

	    // ==================== ENS160 初始化 ====================
	    // ENS160 的 OPMODE 寄存器地址是 0x10。写入 0x02 表示进入标准气体传感模式 (Standard Mode)
	    uint8_t ens_wakeup_cmd[2] = {0x10, 0x02};

	    // 向 ENS160 发送唤醒指令
	    if (HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, ens_wakeup_cmd, 2, 100) != HAL_OK) {
	        // 如果这里返回错误，说明 ENS160 的地址不对，或者引脚没接好
	        // 可以在这里打个断点或者用串口打印个报错
	    }

	    HAL_Delay(100);
}

// 核心读取函数，将读取到的数据解析后存入传入的结构体指针中
void EnvSensor_Read_Data(I2C_HandleTypeDef *hi2c, AHTMESSAGE *msg) {
    uint8_t aht_buf[6];
    uint8_t ens_buf[6];

    // ==================== 读取 AHT21 ====================
    // 发送触发测量命令 (0xAC, 0x33, 0x00)
    uint8_t aht_cmd[3] = {0xAC, 0x33, 0x00};
    HAL_I2C_Master_Transmit(hi2c, AHT21_ADDR, aht_cmd, 3, 100);

    // AHT21 测量需要时间，手册规定大约需要 80ms
    HAL_Delay(80);

    // 读取 6 个字节的数据 (状态字 + 湿度20位 + 温度20位)
    if (HAL_I2C_Master_Receive(hi2c, AHT21_ADDR, aht_buf, 6, 100) == HAL_OK) {
        // 解析温湿度算法
        uint32_t humi_raw = ((uint32_t)aht_buf[1] << 12) | ((uint32_t)aht_buf[2] << 4) | (aht_buf[3] >> 4);
        uint32_t temp_raw = (((uint32_t)(aht_buf[3] & 0x0F)) << 16) | ((uint32_t)aht_buf[4] << 8) | aht_buf[5];

        msg->Humidity = (float)humi_raw * 100.0f / 1048576.0f; // 2^20 = 1048576
        msg->Temperature = (float)temp_raw * 200.0f / 1048576.0f - 50.0f;
    }


    // ==================== 读取 ENS160 ====================
    uint8_t reg_addr = 0x21; // 0x21 是 AQI 寄存器，接着是 TVOC, 然后是 eCO2

        // 先告诉 ENS160：从 0x21 寄存器开始读
        HAL_I2C_Master_Transmit(hi2c, ENS160_ADDR, &reg_addr, 1, 100);

        // 连续读取 6 个字节
        if (HAL_I2C_Master_Receive(hi2c, ENS160_ADDR, ens_buf, 6, 100) == HAL_OK) {
            // ens_buf[0] = AQI (1 字节)
            // ens_buf[1] = TVOC 低字节, ens_buf[2] = TVOC 高字节
            // ens_buf[3] = eCO2 低字节, ens_buf[4] = eCO2 高字节

            msg->AQI  = ens_buf[0];
            msg->TVOC = (ens_buf[2] << 8) | ens_buf[1];
            msg->eCO2 = (ens_buf[4] << 8) | ens_buf[3];
    }
}
