#ifndef __DRIVER_DMA_H
#define __DRIVER_DMA_H
#include "stm32f10x.h"
#include "stdio.h"


extern uint8_t isTransmitFinished;
void Driver_DMA1_Init(void);

void Driver_DMA1_TransimitData(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen);

#endif

