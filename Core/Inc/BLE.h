#ifndef __BLE_H
#define __BLE_H

#include "main.h"

void BLE_Init(void);
void BLE_SendData(uint8_t *data, uint16_t size);
void BLE_RxCallback(uint16_t Size);

#endif /* __BLE_H */
