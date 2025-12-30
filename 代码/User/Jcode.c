#include "bsp_usart.h"
#include <stdlib.h>
char JcodeData; // 全局变量，接收串口数据
char Jdata[10];

/*
J代码格式示例：J1 (+-)30
格式解析：J+电机ID+旋转角度
整体实现流程：接收到串口数据——>状态机判断——>将正确数据存入数组——>将数组对应元素赋值给电机驱动函数
*/

void Jcode_get(uint8_t *Jdata) // 获取J代码指令集
{
        static uint8_t Jstate = 0;
        static uint8_t i = 0;
        uint8_t temp = 0;
        if (Jstate == 0)
        {
                if (JcodeData == 'J')
                {
                        Jstate = 1;
                }
        }
        else if (Jstate == 1)
        {
                temp = (uint8_t)JcodeData - '0'; // 获取电机ID
                Jdata[i] = (uint8_t)temp;
                Jstate = 2;
                i++;
        }
        else if (Jstate == 2)
        {
                if (JcodeData == 0x20) // 遇到空格
                {
                        Jstate = 3;
                }
        }
        else if (Jstate == 3)
        {
                if (JcodeData == '\r') // 遇到换行标志符
                {
                        Jstate = 0;
                        i = 0;
                }
                else
                {
                        Jdata[i] = JcodeData;
                        i++;
                }
        }
}

float Get_J_Angle(void)
{
        float temp;
        temp = strtof((Jdata + 1), NULL);
        return temp;
}
