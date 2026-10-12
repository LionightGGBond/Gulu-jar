#ifndef __CONDUCTIVITY_SENSOR_H
#define __CONDUCTIVITY_SENSOR_H

#include "stm32f1xx_hal.h"

typedef enum {
    COND_STATE_IDLE = 0,
    COND_STATE_BUSY_RX,
    COND_STATE_DATA_READY,
    COND_STATE_ERROR
} Cond_State_t;

extern float T_message;
extern uint16_t E_message;
extern volatile Cond_State_t Cond_State;

void Conductivity_Init(void);
void Conductivity_StartMeasure_IT(void);
void Conductivity_Process(void);

#endif
