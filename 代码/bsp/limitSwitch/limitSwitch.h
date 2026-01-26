#ifndef __LIMITSWITCH_H
#define __LIMITSWITCH_H
#include "stdint.h"
#include "ZDT.h"

#define LimitSwitch_Pin GPIOC
#define Switch1 GPIO_Pin_0
#define Switch2 GPIO_Pin_1
#define Switch3 GPIO_Pin_2
#define Switch4 GPIO_Pin_3
#define Switch5 GPIO_Pin_4
#define EmergencySwitch GPIO_Pin_9

void limitSwitch_Init(void);
uint8_t limitSwitch_Getstatus(uint16_t GPIO_Pin);
uint8_t limitStop(uint16_t GPIO_Pin);
void simple_Stop(void);
#endif
