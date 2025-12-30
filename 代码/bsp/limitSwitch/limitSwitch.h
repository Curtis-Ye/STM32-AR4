#ifndef __LIMITSWITCH_H
#define __LIMITSWITCH_H
#include "stdint.h"

#define limitSwitch_Pin GPIOC
#define switch1 GPIO_Pin_0
#define switch2 GPIO_Pin_1
#define switch3 GPIO_Pin_2
#define switch4 GPIO_Pin_3

void limitSwitch_Init(void);
uint8_t limitSwitch_Getstatus(uint16_t GPIO_Pin);
#endif
