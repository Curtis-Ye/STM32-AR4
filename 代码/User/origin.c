#include <stm32f10x.h>
#include "origin.h"

extern uint8_t limitSwitchMode;
extern uint8_t swStatusArr[5];
#define SWITCHNUM 5

/**
 * @brief 所有轴回归原点并设置原点
 * @param void
 * @retval 1 如果回归原点则返回1
 */
uint8_t All_Go_Origin(void)
{
        limitSwitchMode = 1;
        velocity_parameter origin[5] = {{J1, 1, 10, 10, 1}, {J2, 1, 10, 10, 1}, {J3, 0, 10, 10, 1}, {J4, 1, 10, 10, 1}, {J5, 1, 10, 10, 1}};
        uint8_t originArr[5] = {0, 0, 0, 0, 0};
        uint8_t SwitchArr[SWITCHNUM];
        uint8_t m_state = 1;
        ZDT_Multi_VelocityMotion(5, origin);
        while (m_state)
        {
                SwitchArr[0] = limitStop(Switch1);
                SwitchArr[1] = limitStop(Switch2);
                SwitchArr[2] = limitStop(Switch3);
                SwitchArr[3] = limitStop(Switch4);
                SwitchArr[4] = limitStop(Switch5);
                for (uint8_t i = 0; i < SWITCHNUM; i++)
                {
                        if (SwitchArr[i] != originArr[i])
                        {
                                m_state = 1;
                                break; // 跳出本次循环,进行下一次状态采集
                        }
                        else
                        {
                                m_state = 0;
                        }
                }
        }
        ZDT_SetOrigin(0x00);
        limitSwitchMode = 0;
        return 1;
}

/**
 * @brief 其中一轴回归原点并设置原点
 * @param 电机ID
 * @retval 1 如果回归原点则返回1
 */
uint8_t Go_Origin(uint32_t ID)
{
        uint8_t limitSwitch_State = 1;
        uint16_t GPIO_Pin;
        velocity_parameter origin = {ID, 1, 30, 10, 1};
        switch (ID)
        {
        case J1:
                GPIO_Pin = Switch1;
                break;
        case J2:
                GPIO_Pin = Switch2;
                break;
        case J3:
                GPIO_Pin = Switch3;
                break;
        case J4:
                GPIO_Pin = Switch4;
                break;
        case J5:
                GPIO_Pin = Switch5;
                break;
        }
        if (swStatusArr[ID - 1] != 0)
        {
                ZDT_Multi_VelocityMotion(1, &origin);
        }
        while (limitSwitch_State)
        {
                limitSwitch_State = limitStop(GPIO_Pin);
        }
        ZDT_SetOrigin(ID);
        return 1;
}
