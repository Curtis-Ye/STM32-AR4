#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "stm32f10x.h"
#include "stdio.h"

uint8_t CAN1_Config(void);                                            // 初始化CAN1; 参数：波特率
uint8_t myCANSendData(uint8_t *buf, uint8_t len);
void myCANReceiveData(uint32_t *ID, uint8_t *len, uint8_t* Data);
uint8_t myCANReceiveFlag(void);

#endif
