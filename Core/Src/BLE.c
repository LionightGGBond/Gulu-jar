#include "BLE.h"
#include "usart.h"
// 蓝牙私有接收缓存

	uint8_t ble_rx_buf[128] = {0};

void BLE_Init(void)
{
    // 启动 USART2 的空闲中断+DMA接收
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, ble_rx_buf, sizeof(ble_rx_buf));
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}

void BLE_SendData(uint8_t *data, uint16_t size)
{

    HAL_UART_Transmit_DMA(&huart1, data, size);
}

// 供 main.c 中的中断回调调用的处理函数
void BLE_RxCallback(uint16_t Size)
{
    // 原样回显收到的数据
    BLE_SendData(ble_rx_buf, Size);

    // 重新启动 USART2 接收
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, ble_rx_buf, sizeof(ble_rx_buf));
    __HAL_DMA_DISABLE_IT(huart1.hdmarx, DMA_IT_HT);
}
