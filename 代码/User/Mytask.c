#include "FreeRTOS.h"
#include "task.h"
#include "Init.h"
// 使用动态创建任务
extern uint8_t USART1_Status;
// 任务句柄
TaskHandle_t Stop_TaskHandle;
TaskHandle_t USART1_TaskHandle;
TaskHandle_t LED_TaskHandle;

void Stop_Task(void)
{
        while (1)
        {
                simple_Stop();
        }
}

void USART1_Task(void)
{
        while (1)
        {
                if (USART1_Status == 1)
                {
                        USART1_Data_process();
                }
        }
}

void LED_Task(void)
{
        while (1)
        {
                LED_ON();
                vTaskDelay(500);
                LED_OFF();
                vTaskDelay(500);
        }
}

void createMyTask(void)
{
        xTaskCreate((TaskFunction_t)Stop_Task,
                    (const char *)"Stop_Task",
                    (uint32_t)512,
                    (void *)NULL,
                    (UBaseType_t)4,
                    (TaskHandle_t *)&Stop_TaskHandle);
        xTaskCreate((TaskFunction_t)USART1_Task,
                    (const char *)"USART1_Task",
                    (uint32_t)512,
                    (void *)NULL,
                    (UBaseType_t)4,
                    (TaskHandle_t *)&USART1_TaskHandle);
        xTaskCreate((TaskFunction_t)LED_Task,
                    (const char *)"LED_Task",
                    (uint32_t)128,
                    (void *)NULL,
                    (UBaseType_t)4,
                    (TaskHandle_t *)&LED_TaskHandle);
        vTaskStartScheduler();
}
