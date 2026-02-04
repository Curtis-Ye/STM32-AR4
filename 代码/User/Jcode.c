#include <stdlib.h>
#include "Jcode.h"

char JcodeData; // 全局变量，接收串口数据
char Jdata[25]; // 串口数据存储数组
extern uint8_t USART1_Status;
extern position_parameter parameter1;

/*
J代码格式示例：J1 (+-)xx Vxx Axx 1/0
格式解析：J+电机ID+旋转角度+速度+加速度+正反转
整体实现流程：接收到串口数据——>将正确数据存入数组——>解析对应数据——>发送电机控制指令
*/

/**
 * @brief 处理串口接收到的数据
 * @param void
 * @retval void
 */
void USART1_Data_process(void)
{
        char *str;
        if (Jdata[0] == 'J')
        {
                parameter1.ID = (uint8_t)(Jdata[1] - '0');
                str = Jdata + 3;
                char *p = NULL;
                parameter1.angle = strtof(str, &p);
                str = p + 2;
                parameter1.vel = strtof(str, &p);
                str = p + 2;
                parameter1.ac = strtof(str, &p);
                str = p + 1;
                parameter1.mode = strtof(str, &p);
                ZDT_Pos_Control(&parameter1);
                USART1_Status = 0;
        }
        else if (Jdata[0] == 'F')
        {
                if (Jdata[1] == '0' && Jdata[3] == '\r')
                {
                        if (All_Go_Origin())
                        {
                                USART1_Status = 0;
                        }
                }
                else if (Jdata[1] == '0' && Jdata[3] == 'J')
                {
                        str = Jdata + 4;
                        uint8_t id = strtof(str, NULL);
                        if (Go_Origin(id))
                        {
                                USART1_Status = 0;
                        }
                }
        }
}
