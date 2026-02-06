#ifndef __MYTASK_H
#define __MYTASK_H
#include "FreeRTOS.h"
#include "task.h"
void Stop_Task(void);
void USART1_Task(void);
void createMyTask(void);
void LED_Task(void);
#endif
